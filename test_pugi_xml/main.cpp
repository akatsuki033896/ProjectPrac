#include "pugixml/pugixml.hpp"
#include <iostream>
#include <cstring>

void find_node() {
    const char* xml =
        "<root>"
        "    <name>hello</name>"
        "</root>";
    pugi::xml_document doc;
    
    // pugi::xml_parse_result result = doc.load_file("test.xml");
    pugi::xml_parse_result result = doc.load_string(xml);
    
    // if (!result) {
    //     std::cout << "XML parsed with errors, error description: " << result.description() << "\n";
    //     std::cout << "Error offset: " << result.offset << '\n';
    //     return 1;
    // }
    // std::cout << "XML Parse success. Root node: " << doc.document_element().name() << "\n";
    
    auto find_node = doc.find_node([] (pugi::xml_node& node) {
        return strcmp(node.name(), "name") == 0;
    });
    std::cout << "Found node: " << find_node.name();
    return;
}

void build_xml(pugi::xml_document& doc) {
    pugi::xml_node root = doc.append_child("Dataset");
    root.append_attribute("xmlns") = "http://www.iec.ch/S421/2.0";

    pugi::xml_node name1 = root.append_child("name1");
    name1.append_child(pugi::node_pcdata).set_value("hello");

    pugi::xml_node name2 = root.append_child("name2");
    // name2.set_value("world"); 无效
    name2.append_child(pugi::node_pcdata).set_value("world");

    pugi::xml_node name3 = name1.append_child("name3");
    name3.append_child(pugi::node_pcdata).set_value("xml");
    return;
}

int main() {
    pugi::xml_document doc;
    build_xml(doc);
    doc.save(std::cout, "  "); // 终端输出
    doc.save_file("../output.gml", "  ");
    return 0;
}