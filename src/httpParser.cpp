#include "httpParser.hpp"

std::string HttpParser::send200OK() {
  std::string okString = "HTTP/1.1 200 OK\r\n\r\n";
  return okString;
}

std::map<std::string, std::string> HttpParser::httpResMap(std::string response) {
  std::map<std::string, std::string> responseMap;
  size_t pos = 0;
  std::string delimiter = "\n";
  std::string token;
  std::string firstLine;
  response.find_first_not_of(delimiter);
  pos = response.find(delimiter);
  // find first line to check if it's response or request
  firstLine = response.substr(0, pos);
  // response HTTP/ will be at the begining of the first line
  int firstLine_pos = firstLine.find("HTTP/");
  if (firstLine_pos == 0) {
    responseMap.insert({"Type", "Response"});
    firstLine_pos = firstLine.find(" ");
    responseMap.insert(
        {"StatusCode", firstLine.substr(firstLine_pos + 1, firstLine.length())});
  }
  else if (firstLine_pos > 0) {
    responseMap.insert({"Type", "Request"});
    firstLine_pos = firstLine.find(" ");
    responseMap.insert({"Method", firstLine.substr(0, firstLine_pos + 1)});
  }

  // get header
  response.erase(0, pos + delimiter.length());
  std::string header = "";
  while ((pos = response.find(delimiter)) != 1 &&
         response.find(delimiter) != std::string::npos) {
    token = response.substr(0, pos);
    header += token + "\n";
    response.erase(0, pos + delimiter.length());
  }
  response.erase(0, pos + delimiter.length());
  // body
  responseMap.insert({"Body", response});
  // parse header, each attribute before : will be used as key
  std::string headerDeli = ":";
  std::string tokenKey;
  std::string tokenValue;
  while ((pos = header.find(headerDeli)) != std::string::npos) {
    tokenKey = header.substr(0, pos);
    // cout << tokenKey << endl;
    header.erase(0, pos + headerDeli.length());
    pos = header.find(delimiter);
    tokenValue = header.substr(1, pos - 1);
    // cout << tokenValue << endl;
    header.erase(0, pos + delimiter.length());
    responseMap.insert({tokenKey, tokenValue});
  }

  if (responseMap["Type"] == "Request") {
    if ((pos = responseMap["Host"].find(":")) != std::string::npos) {
      std::string tmp = responseMap["Host"];
      responseMap["Host"] = responseMap["Host"].substr(0, pos);
      responseMap.insert({"Port", tmp.substr(pos + 1, responseMap["Host"].length())});
    }
    else {
      responseMap.insert({"Port", "-1"});
    }
  }
  if ((pos = responseMap["Host"].find("\r")) != std::string::npos) {
    responseMap["Host"] = responseMap["Host"].substr(0, pos);
  }
  return responseMap;
}
