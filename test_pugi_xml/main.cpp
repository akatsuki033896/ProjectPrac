#include "pugixml/pugixml.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

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
    root.append_attribute("gml:id") = "S421.abc.abcde.00001";

    pugi::xml_node bound = root.append_child("gml:boundedBy");
    
    // gml:Envelope
    auto envelope = bound.append_child("gml:Envelope");
    envelope.append_attribute("srsName") = "http://www.opengis.net/def/crs/EPSG/0/4326";
    envelope.append_attribute("srsDimension") = "2";
    envelope.append_child("gml:lowerCorner");
    envelope.append_child("gml:upperCorner");

    pugi::xml_node s100 = root.append_child("S100:DatasetIdentificationInformation");
    s100.append_child("S100:productIdentifier").text().set("S-421");

    return;
}

void func() {
    std::ostringstream oss;
    oss << 123 << " hello";
    auto str = oss.str();
    std::cout << str << std::endl;
}

int main() {
    // pugi::xml_document doc;
    // build_xml(doc);
    
    // doc.save(std::cout, "    "); // 终端输出
    // doc.save_file("../output.xml", "    ");


    // string test
    func();
    return 0;
}