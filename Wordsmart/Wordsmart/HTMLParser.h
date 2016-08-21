#pragma once
#include <string>
#include <vector>
#include <map>
#include <codecvt>
#include <locale>
#include <sstream>

using std::wstring;
using std::map;
using std::vector;

namespace Parser {
	class DOMTree;

	struct HTMLElement
	{
		wstring tag;
		map<wstring, wstring> attr;
		wstring inner_text;

		wstring _inner_html;
		size_t tag_start;
		size_t tag_end;

		HTMLElement* parent;
		vector<HTMLElement*> children;

		DOMTree* dom;

		HTMLElement(DOMTree* dom) : dom(dom) {

		}
		HTMLElement(DOMTree* dom, wstring& tag, map<wstring, wstring>& attr) : dom(dom), tag(tag), attr(attr) {

		}

		HTMLElement(const HTMLElement& e) : dom(e.dom), tag(e.tag), attr(e.attr) {

		}

		inline wstring inner_html();
	};

	class DOMTree
	{
		HTMLElement* top;
		HTMLElement* cursor;
		wstring html;

		bool delete_element(HTMLElement* e) {
			for (int i = 0; i < e->children.size(); i++) {
				delete_element(e->children[i]);
			}
			delete e;
		}

	public:
		class iterator {
			HTMLElement* curr;
			DOMTree& dom;

		public:
			bool operator!=(const iterator& itr) {
				if (curr != itr.curr) return true;
				return false;
			}
			HTMLElement* operator->() {
				return curr;
			}

			void operator++() {
				// top is the end iterator
				if (curr == dom.top) {
					return;
				}

				// If it has a children, we go to the lower level
				if (curr->children.size()) {
					curr = curr->children[0];
					return;
				}
				else {
					HTMLElement* parent = curr->parent;
					while (true) {
						for (size_t i = 0; i < parent->children.size(); i++) {
							if (parent->children[i] == curr) {
								if (i < parent->children.size() - 1) {
									curr = parent->children[i + 1];
									return;
								}
								else {
									if (parent == dom.top) {
										curr = parent;
										return;
									}
									curr = parent;
									parent = curr->parent;
								}
							}
						}
					}
				}
			}
			iterator(HTMLElement* curr, DOMTree& dom) : curr(curr), dom(dom) {

			}

			iterator& operator=(iterator& itr) {
				curr = itr.curr;
				dom = itr.dom;
				return *this;
			}

		};

		DOMTree() {
			// Top level DOM Element (has no data)
			// Only to specify the top-most level
			top = new HTMLElement(this);
			cursor = top;
		}

		void set_html(wstring& h) {
			html = h;
		}

		bool level_up() {
			if (cursor == top) return false;

			cursor = cursor->parent;
			return true;
		}
		// To Fully create a DOM Tree, 
		// whenever the parser reads opening tag, use add_element() to push a new Element.
		// whenever the parser reads closing tag, use level_up() to go upper level
		void add_element(HTMLElement *e) {
			cursor->children.push_back(e);

			e->parent = cursor;
			cursor = e;
		}
		// Do not move a cursor
		void add_void_element(HTMLElement *e) {
			cursor->children.push_back(e);
			e->parent = cursor;
		}
		void add_text(wstring text) {
			if (cursor == top) return;
			cursor->inner_text += text;
		}

		void set_html_start_location(size_t start) {
			cursor->tag_start = start;
		}
		void set_inner_html(wstring& s, size_t end) {
			cursor->tag_end = end;
		}

		iterator begin() {
			if (top->children.size() == 0) return iterator(top, *this);

			return iterator(top->children[0], *this);
		}

		iterator end() {
			return iterator(top, *this);
		}

		wstring& get_html() {
			return html;
		}

		~DOMTree() {
		}
	};

	inline wstring HTMLElement::inner_html() {
		if (!_inner_html.size()) {
			_inner_html = dom->get_html().substr(tag_start, tag_end - tag_start + 1);
		}
		return _inner_html;
	}

	class HTMLParser
	{
		wstring html;
		DOMTree dom_tree;

		bool is_void_element(wstring tag) {
			vector<wstring> arr = { L"area", L"base", L"br", L"br/", L"col", L"command",
				L"embed", L"hr", L"img", L"input", L"keygen", L"link",
				L"meta", L"param", L"source", L"track", L"wbr" };

			for (size_t i = 0; i < arr.size(); i++) {
				if (tag == arr[i]) return true;
			}
			return false;
		}

	public:
		HTMLParser(wstring html_str) : html(html_str) {
			strip_whitespace(html);
			dom_tree.set_html(html);

			wstring open_bracket = L"<";
			wstring closing_bracket = L">";

			auto found = html.find(open_bracket);
			while (found != wstring::npos) {
				auto end = html.find(closing_bracket, found + 1);
				HTMLElement e = parse_tag(html.substr(found + 1, end - found - 1));

				// If tag name is empty, then it is a closing tag
				if (e.tag == L"") {
					dom_tree.set_inner_html(html, found - 1);
					dom_tree.level_up();
				}
				else if (is_void_element(e.tag)) {
					dom_tree.add_void_element(new HTMLElement(e));
				}
				else if (e.tag != L"!--") {
					dom_tree.add_element(new HTMLElement(e));
					dom_tree.set_html_start_location(end + 1);
				}
				found = html.find(open_bracket, end + 1);

				if (found != end + 1) {
					// Then this area is a innerText of current tag
					wstring temp = html.substr(end + 1, found - (end + 1));
					remove_trailing_whitespace(temp);
					dom_tree.add_text(temp);
				}
			}
		}

		void strip_whitespace(wstring& s) {
			if (s.size() == 0) return;

			// size_t is unsigned int 
			for (size_t i = s.size() - 1; i >= 0; i--) {
				if (s[i] == L'\t' || s[i] == L'\n' || s[i] == L'\r') {
					s.erase(s.begin() + i);
				}
				if (i == 0) break;
			}
		}

		void remove_trailing_whitespace(wstring& s) {
			if (s.size() == 0) return;

			for (int i = s.length() - 1; i >= 0; i--) {
				if (s[i] == ' ') s.erase(s.begin() + i);
				else break;
			}
		}
		
		HTMLElement parse_tag(const wstring& s) {
			vector<wstring> split;
			map<wstring, wstring> attr;

			std::wstringstream ws(s);
			while (ws.good()) {
				wstring temp; ws >> temp;
				split.push_back(temp);
			}

			// If it is a closing tag
			if (split[0][0] == L'/') {
				return HTMLElement(&dom_tree);
			}
			else if (split[0] == L"!--") {
				return HTMLElement(&dom_tree, split[0], attr);
			}

			wstring tag_name = L"";
			if (split.size()) {
				tag_name = split[0];
			}

			// There are following possibilities for the attribute composition
			// style="background: blue;" [style="backgroud:, blue"]  - CASE 1
			// style ="background: blue;" [style, ="background:, blue;"]  - CASE 2
			// style= "background: blue;" [style=, "background:, blue;"] - CASE 3
			// style = "background: blue;" [style, =, "background:, blue;"] - CASE 4

			wstring attr_name;
			bool attr_found = false;

			bool equal_found = false;

			wstring value;
			bool value_start_found = false;
			wchar_t quote;

			size_t i, j;
			for (i = 1; i < split.size(); i++) {
				if (!attr_found) {
					attr_found = true;

					for (j = 0; j < split[i].size(); j++) {
						if (split[i][j] == L'=') {
							attr_name = split[i].substr(0, j);
							equal_found = true;

							break;
						}
					}

					// if "=" is not found
					if (!attr_name.size()) {
						attr_name = split[i];
					}

					j++;

					if (j < split[i].size()) { // CASE 1
						if (split[i][j] == L'\'' || split[i][j] == L'\"') {
							quote = split[i][j];

							if (j != split[i].size() - 1 && split[i][split[i].size() - 1] == quote) {
								value = split[i].substr(j + 1, split[i].size() - j - 2);
								attr[attr_name] = value;

								attr_name = L"";
								value = L"";

								attr_found = false;
								equal_found = false;
							}
							else {
								value = split[i].substr(j + 1);
								value_start_found = true;
							}
						}
						else {
							value = split[i].substr(j);
							attr[attr_name] = value;

							attr_name = L"";
							value = L"";

							attr_found = false;
							equal_found = false;
						}
					}
				}
				else {
					j = 0;
					if (!equal_found) {
						if (split[i][j] == L'=') {
							equal_found = true;
							j++;
						}
						else {
							attr[attr_name] = L"";

							attr_name = L"";
							value = L"";

							attr_found = false;
						}
					}

					if (j == split[i].size()) continue;

					if (!value_start_found) {
						if (split[i][j] == L'\'' || split[i][j] == L'\"') {
							quote = split[i][j];

							if (j != split[i].size() - 1 && split[i][split[i].size() - 1] == quote) {
								value = split[i].substr(j + 1, split[i].size() - j - 2);
								attr[attr_name] = value;

								attr_name = L"";
								value = L"";

								attr_found = false;
								equal_found = false;
							}
							else {
								value = split[i].substr(j + 1);
								value_start_found = true;
							}
						}
						else {
							value = split[i].substr(j);
							attr[attr_name] = value;

							attr_name = L"";
							value = L"";

							attr_found = false;
							equal_found = false;
						}
					}
					else {
						value += L" " + split[i].substr(j);

						if (value[value.size() - 1] == quote) {
							value.pop_back(); // Last character is quote

							attr[attr_name] = value;

							attr_found = false;
							equal_found = false;
							value_start_found = false;
						}
					}
				}

			}

			return HTMLElement(&dom_tree, tag_name, attr);
		}
		DOMTree& DOM() { return dom_tree;  }


	};
}