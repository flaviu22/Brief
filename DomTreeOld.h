#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cctype>

class CDomTree
{
public:
	struct Attribute
	{
	public:
		std::string m_key{};
		std::string m_value{};
	};

	struct Tag
	{
	public:
		Tag() {};
		Tag(const std::string& name)
			:m_name(name)
		{}
		Tag(const std::string& name, const std::string& value)
			:m_name(name)
			,m_value(value)
		{}
		Tag(const std::string& name, const std::string& value, const std::vector<Attribute>& attributes)
			:m_name(name)
			,m_value(value)
			,m_attributes(attributes)
		{}
		Tag(const std::string& name, const std::vector<Attribute>& attributes)
			:m_name(name)
			,m_attributes(attributes)
		{}
		Tag(std::string&& name)
			:m_name(std::move(name))
		{}
		Tag(std::string&& name, std::string&& value)
			:m_name(std::move(name))
			,m_value(std::move(value))
		{}
		Tag(std::string&& name, std::string&& value, std::vector<Attribute>&& attributes)
			:m_name(std::move(name))
			,m_value(std::move(value))
			,m_attributes(std::move(attributes))
		{}
		Tag(std::string&& name, std::vector<Attribute>&& attributes)
			:m_name(std::move(name))
			,m_attributes(std::move(attributes))
		{}
		Tag(const Tag& rhs)
		{
			*this = rhs;
		}
		Tag& operator=(const Tag& rhs)
		{
			if (this != &rhs)
			{
				m_parent = rhs.m_parent;
				m_name = rhs.m_name;
				m_value = rhs.m_value;
				m_childs = rhs.m_childs;
				m_attributes = rhs.m_attributes;
			}
			return *this;
		}
		Tag(Tag&& rhs) noexcept
		{
			*this = std::move(rhs);
		}
		Tag& operator=(Tag&& rhs) noexcept
		{
			if (this != &rhs)
			{
				m_parent = rhs.m_parent;
				m_name = std::move(rhs.m_name);
				m_value = std::move(rhs.m_value);
				m_childs = std::move(rhs.m_childs);
				m_attributes = std::move(rhs.m_attributes);
				rhs.m_parent = nullptr;
			}
			return *this;
		}

	public:
		mutable Tag* m_parent{};
		std::string m_name{};
		std::string m_value{};
		std::vector<Tag> m_childs{};
		std::vector<Attribute> m_attributes{};

	public:
		void AddAttributes(const std::vector<Attribute>& attributes)
		{
			std::copy(std::begin(attributes), std::end(attributes), std::back_inserter(m_attributes));
		}
		void AddAttributes(std::vector<Attribute>&& attributes)
		{
			if (m_attributes.empty())
			{
				m_attributes = std::move(attributes);
			}
			else
			{
				m_attributes.reserve(m_attributes.size() + attributes.size());
				std::move(std::begin(attributes), std::end(attributes), std::back_inserter(m_attributes));
				attributes.clear();
			}
		}
		void SetValue(const std::string& text)
		{
			m_value = text;
		}
		void SetValue(std::string&& text)
		{
			m_value = std::move(text);
		}
		void AddText(const std::string& text)
		{
			Tag tag{};
			tag.m_value = text;
			tag.m_parent = this;
			m_childs.push_back(tag);
		}
		void AddText(std::string&& text)
		{
			Tag tag{};
			tag.m_value = std::move(text);
			tag.m_parent = this;
			m_childs.push_back(std::move(tag));
		}
		void AddChild(const Tag& tag)
		{
			tag.m_parent = this;
			m_childs.push_back(tag);
		}
		void AddChild(Tag&& tag)
		{
			tag.m_parent = this;
			m_childs.push_back(std::move(tag));
		}
	};

public:
	CDomTree() = default;
	CDomTree(const CDomTree& rhs) = delete;
	CDomTree& operator=(const CDomTree& rhs) = delete;
	CDomTree(CDomTree&& rhs) noexcept
	{
		*this = std::move(rhs);
	}
	CDomTree& operator=(CDomTree&& rhs) noexcept
	{
		if (this != &rhs)
		{
			m_currentTag = rhs.m_currentTag;
			m_data = std::move(rhs.m_data);
			m_bufferIndex = rhs.m_bufferIndex;
			m_tags = std::move(rhs.m_tags);
			m_svg = rhs.m_svg;
			m_script = rhs.m_script;
			rhs.m_currentTag = nullptr;
			rhs.m_bufferIndex = 0;
			rhs.m_svg = false;
			rhs.m_script = false;
		}
		return *this;
	}
	~CDomTree() = default;

public:
	std::vector<Tag>& GetTags() { return m_tags; }
	const std::vector<Tag>& GetTags() const { return m_tags; }
	void Parse(const std::string_view& data)
	{
		m_data = data;
		Parse();
	}
	void Parse(std::string&& data)
	{
		m_data = std::move(data);
		Parse();
	}

	std::string GetData()
	{
		std::string out{};
		PrintData(m_tags, out);
		return out + "\n";
	}

private:
	void Parse()
	{
		m_bufferIndex = 0;
		while (m_bufferIndex < m_data.length())
		{
			if (!ParseNextToken())
				break;
		}
	}

	bool ParseNextToken()
	{
		if (m_bufferIndex >= m_data.length())
			return false;

		if (!m_currentTag || !m_script || !m_svg)
			SkipWhiteSpaces();

		if (m_svg)
			return ParseValue();

		if ('<' == m_data[m_bufferIndex])
			return ParseTag();
		else
			return ParseValue();
	}

	bool ParseTag()
	{
		if (m_bufferIndex >= m_data.length())
			return false;

		m_bufferIndex++;

		if (m_bufferIndex >= m_data.length())
			return false;

		switch (m_data[m_bufferIndex])
		{
		case '/':
			m_bufferIndex++;
			if (!ParseClosingTag())
				return false;
			break;
		case '!':
		case '?':
			if ('!' == m_data[m_bufferIndex]
				&& m_bufferIndex < m_data.length() - 3
				&& '-' == m_data[m_bufferIndex + 1]
				&& '-' == m_data[m_bufferIndex + 2])
				ParseCommentTag();
			else
				ParseSpecialTag();
			break;
		default:
			if (!ParseOpeningTag())
				return false;
		}

		return true;
	}

	bool ParseValue()
	{
		if (!m_currentTag)
			return false;

		std::string value{};

		do
		{
			if (!m_script && !m_svg)
			{
				while (m_bufferIndex < m_data.length() - 1 && '<' != m_data[m_bufferIndex])
					value += m_data[m_bufferIndex++];
				break;
			}

			if (m_script)
			{
				while (m_bufferIndex < m_data.length() - 5
					&& !('<' == m_data[m_bufferIndex]
						&& '/' == m_data[m_bufferIndex + 1]
						&& 's' == m_data[m_bufferIndex + 2]
						&& 'c' == m_data[m_bufferIndex + 3]
						&& 'r' == m_data[m_bufferIndex + 4]))
					value += m_data[m_bufferIndex++];
				m_script = false;
				break;
			}

			if (m_svg)
			{
				while (m_bufferIndex < m_data.length() - 5
					&& !('<' == m_data[m_bufferIndex]
						&& '/' == m_data[m_bufferIndex + 1]
						&& 's' == m_data[m_bufferIndex + 2]
						&& 'v' == m_data[m_bufferIndex + 3]
						&& 'g' == m_data[m_bufferIndex + 4]))
					value += m_data[m_bufferIndex++];
				m_svg = false;
			}
		} while (false);

		Tag tag{};
		tag.m_parent = m_currentTag;
		tag.m_value = value;
		m_currentTag->m_childs.push_back(std::move(tag));

		return true;
	}

	bool ParseSpecialTag()
	{
		Tag tag{};

		while (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex])
			tag.m_name += m_data[m_bufferIndex++];

		if (m_tags.empty() || !m_currentTag)
		{
			m_tags.push_back(tag);
		}
		else
		{
			tag.m_parent = m_currentTag;
			tag.m_parent->m_childs.push_back(tag);
		}

		if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
			m_bufferIndex++;

		return true;
	}

	bool ParseCommentTag()
	{
		Tag tag{};

		while (m_bufferIndex < m_data.length() - 3 &&
			!('>' == m_data[m_bufferIndex]
				&& '-' == m_data[m_bufferIndex - 1]
				&& '-' == m_data[m_bufferIndex - 2]))
			tag.m_name += m_data[m_bufferIndex++];

		if (m_tags.empty() || !m_currentTag)
		{
			m_tags.push_back(tag);
		}
		else
		{
			tag.m_parent = m_currentTag;
			tag.m_parent->m_childs.push_back(tag);
		}

		if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
			m_bufferIndex++;

		return true;
	}

	bool ParseOpeningTag()
	{
		SkipWhiteSpaces();

		Tag tag{};
		// parse tag name
		while (m_bufferIndex < m_data.length()
			&& !IsWhiteSpace(m_data[m_bufferIndex])
			&& '>' != m_data[m_bufferIndex]
			&& '/' != m_data[m_bufferIndex])
			tag.m_name += std::tolower(m_data[m_bufferIndex++]);

		if (m_tags.empty() || !m_currentTag)
		{
			m_tags.push_back(tag);
			m_currentTag = &m_tags.back();
		}
		else
		{
			tag.m_parent = m_currentTag;
			tag.m_parent->m_childs.push_back(tag);
			m_currentTag = &tag.m_parent->m_childs.back();

			m_script = ("script" == m_currentTag->m_name);
			if (!m_script)
				m_svg = ("svg" == m_currentTag->m_name);
		}

		ParseAttributes();

		if (non_closing_tags_.cend() != std::find(non_closing_tags_.cbegin(), non_closing_tags_.cend(), tag.m_name))
			m_currentTag = m_currentTag->m_parent;

		if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
			m_bufferIndex++;

		return true;
	}

	bool ParseClosingTag()
	{
		SkipWhiteSpaces();

		while (m_bufferIndex < m_data.length() && !IsWhiteSpace(m_data[m_bufferIndex]) && '>' != m_data[m_bufferIndex])
			m_bufferIndex++;

		if ('>' == m_data[m_bufferIndex] || m_data[m_bufferIndex] >= m_data.length())
			m_bufferIndex++;

		if (m_currentTag)
			m_currentTag = m_currentTag->m_parent;

		return true;
	}

	void ParseAttributes()
	{
		while (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex])
		{
			SkipWhiteSpaces();

			if (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex] && '/' != m_data[m_bufferIndex])
			{
				std::string key{}, value{};
				while (m_bufferIndex < m_data.length()
					&& '=' != m_data[m_bufferIndex]
					&& '>' != m_data[m_bufferIndex]
					&& !IsWhiteSpace(m_data[m_bufferIndex]))
					key += m_data[m_bufferIndex++];

				SkipWhiteSpaces();
				if (m_bufferIndex < m_data.length() && '=' == m_data[m_bufferIndex])
				{
					m_bufferIndex++;
					SkipWhiteSpaces();
					if (m_bufferIndex < m_data.length() && ('\"' == m_data[m_bufferIndex] || '\'' == m_data[m_bufferIndex]))
					{
						const char quote = m_data[m_bufferIndex++];
						while (m_bufferIndex < m_data.length() && quote != m_data[m_bufferIndex])
							value += m_data[m_bufferIndex++];
					}
				}
				else
				{
					m_bufferIndex--;
				}
				m_currentTag->m_attributes.push_back(Attribute{ key, value });
			}

			if ('>' != m_data[m_bufferIndex])
				m_bufferIndex++;
		}
	}

	void SkipWhiteSpaces()
	{
		while (m_bufferIndex < m_data.length() && IsWhiteSpace(m_data[m_bufferIndex]))
			m_bufferIndex++;
	}
	constexpr bool IsWhiteSpace(const char c) const
	{
		return (' ' == c || '\r' == c || '\n' == c || '\t' == c);
	}

	std::string GetIndent(size_t tabs) const
	{
		std::ostringstream buffer{};
		while (tabs--)
			buffer << "\t";
		return buffer.str();
	}

	void PrintData(const std::vector<Tag>& tags, std::string& data, const size_t level = 0) const
	{
		for (const auto& it : tags)
		{
			if (it.m_name.empty())
				continue;

			if ('!' != it.m_name.front() && '?' != it.m_name.front())
			{
				PrintName(it, data, level);
				PrintValue(it, data, level);
				PrintClose(it, data, level);
			}
			else
			{
				if (!data.empty())
					data += "\n";
				data += GetIndent(level) + "<" + it.m_name + ">";
			}
		}
	}

	void PrintName(const Tag& tag, std::string& data, const size_t level) const
	{
		if (!data.empty())
			data += "\n";
		data += GetIndent(level) + "<" + tag.m_name;
		for (const auto& attr : tag.m_attributes)
		{
			data += " " + attr.m_key;
			if (!attr.m_value.empty())
				data += "=\"" + attr.m_value + "\"";
		}
		data += ">";
	}

	void PrintValue(const Tag& tag, std::string& data, const size_t level) const
	{
		if (0 == tag.m_childs.size())
		{
			data += tag.m_value.substr(0, tag.m_value.find_last_not_of(m_whitespace) + 1);
			return;
		}

		for (const auto& it : tag.m_childs)
		{
			if (it.m_name.empty())	// is value
			{
				if (1 == tag.m_childs.size() &&
					tag.m_childs.front().m_name.empty() &&
					"script" != tag.m_name &&
					"svg" != tag.m_name &&
					"style" != tag.m_name)
					data += it.m_value.substr(0, it.m_value.find_last_not_of(m_whitespace) + 1);
				else
					data += "\n" + GetIndent(level + 1) + it.m_value.substr(0, it.m_value.find_last_not_of(m_whitespace) + 1);
			}
			else
			{
				PrintData({ it }, data, level + 1);
			}
		}
	}

	void PrintClose(const Tag& tag, std::string& data, const size_t level) const
	{
		if (non_closing_tags_.cend() == std::find(non_closing_tags_.cbegin(), non_closing_tags_.cend(), tag.m_name))
		{
			if (tag.m_childs.size() > 1 || (0 != tag.m_childs.size() && !tag.m_childs.front().m_name.empty()) ||
				"script" == tag.m_name || "svg" == tag.m_name || "style" == tag.m_name)
				data += "\n" + GetIndent(level);
			data += "</" + tag.m_name + ">";
		}
	}

private:
	Tag* m_currentTag{};
	std::string m_data{};
	size_t m_bufferIndex{};
	std::vector<Tag> m_tags{};
	bool m_svg{ false };
	bool m_script{ false };
	static constexpr std::string_view m_whitespace{ " \n\r\t" };

private:
	const std::vector<std::string> non_closing_tags_
	{
		{"area"},
		{"base"},
		{"br"},
		{"col"},
		{"command"},
		{"embed"},
		{"hr"},
		{"img"},
		{"input"},
		{"keygen"},
		{"link"},
		{"meta"},
		{"param"},
		{"source"},
		{"track"},
		{"wbr"}
	};
};
