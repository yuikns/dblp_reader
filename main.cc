/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Start Example:
 * bazel run //app/dblp_reader:paper_collecter -- --in=$(pwd)/app/dblp_reader/data/dblp.xml --out=$(pwd)/publications.json
 *
 **/
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stack>
#include <string>
#include <vector>

#include "argcv/c/char/char_helper.h"
#include "argcv/cxx/helper/xml_sax_parser.h"
#include "argcv/cxx/str/str.h"
#include "argcv/cxx/platform/timer.h"
#include "gflags/gflags.h"
#include "gflags/gflags_completions.h"

#include "third_party/njson/json.h"

using argcv::helper::XmlSAXParser;
using argcv::helper::XmlAttrElems;
using argcv::helper::XmlAttrPairs;
using argcv::platform::Timer;
using argcv::str::StrSlice;

using json = nlohmann::json;

DEFINE_string(in, "app/dblp_reader/data/dblp-sample.xml", "input file");
DEFINE_string(out, "dblp_ext_publications.json", "output file");
DEFINE_string(name, "Jimeng Sun", "name to be filtered");
DEFINE_bool(h, false, "Show help");

const static std::map<const std::string, int> kLevelDict{
    {"article", 1},       {"inproceedings", 1}, {"proceedings", 1},
    {"book", 1},          {"incollection", 1},  {"phdthesis", 1},
    {"mastersthesis", 1}, {"www", 1},           {"author", 2},
    {"editor", 2},        {"title", 2},         {"booktitle", 2},
    {"pages", 2},         {"year", 2},          {"address", 2},
    {"journal", 2},       {"volume", 2},        {"number", 2},
    {"month", 2},         {"url", 2},           {"ee", 2},
    {"cdrom", 2},         {"cite", 2},          {"publisher", 2},
    {"note", 2},          {"crossref", 2},      {"isbn", 2},
    {"series", 2},        {"school", 2},        {"chapter", 2},
};

int GetLevel(const std::string &name) {
  if (kLevelDict.find(name) == kLevelDict.end()) {
    return 0;
  } else {
    return kLevelDict.at(name);
  }
}

std::string NameFix(const std::string &name) {
  int i = name.length() - 1;
  for (; i >= 0; i--) {
    if (is_digit(name[i])) {
      // LOG(INFO) << "is dight...[" << name[i] << "]";
      continue;
    }
    if (is_western_space(name[i])) {
      // LOG(INFO) << "is space...[" << name[i] << "]";
      continue;
    }
    break;
  }
  i++;
  // LOG(INFO) << "LEN:" << i;
  return StrSlice(name).Take(i).ToString();
}

int main(int argc, char *argv[]) {
  gflags::SetVersionString("0.0.1");
  gflags::SetUsageMessage("Usage : ./paper_collecter ");
  gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);

  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  FILE *f = fopen(FLAGS_in.c_str(), "r");
  if (!f) {
    LOG(FATAL) << "file open error." << FLAGS_in;
  }

  // std::string name_on_watch = "Jimeng Sun";
  // std::string name_on_watch("E. F. Codd");

  LOG(INFO) << "looking up: " << FLAGS_name;

  json doc;

  doc["name"] = FLAGS_name;
  doc["pubs"] = json::array();

  json pub;
  json author;

  Timer m_tm;
  std::stack<int> lv;
  lv.push(0);
  std::string curr_name = "";
  std::string field_name = "";
  StrSlice sslc;
  bool is_valid_pub = false;
  XmlSAXParser handler;
  handler
      .AddDTDSystemURIMapping("dblp-2017-08-29.dtd",
                              "http://dblp.uni-trier.de/xml/dblp.dtd")
      .Option(XML_PARSE_NOBLANKS | XML_PARSE_NOCDATA | XML_PARSE_COMPACT |
              XML_PARSE_NSCLEAN)
      .OnStartElement([&](const std::string &name, const XmlAttrPairs &attrs) {
        int clv = GetLevel(name);
        if (lv.top() >= 2) {
          clv = 3;
        }
        lv.push(clv);
        // LOG(INFO) << "enqueue:" << name << "(" << clv << ")";
        switch (clv) {
          case 1:
            LOG_IF_EVERY_N(INFO, (name != "www"), 100000)
                << "name[" << name << "], n_pubs:" << doc["pubs"].size()
                << "time spending:" << m_tm.All().Ms() << "ms";

            curr_name = name;
            // cleanup
            pub = json::object();
            author = json::array();
            is_valid_pub = false;
            if (curr_name != "www") {
              json jattr;
              for (const auto attr : attrs) {
                jattr[attr.first] = attr.second;
              }
              if(jattr.at("key") != nullptr) {
                pub["trace_id"] = jattr.at("key").get<std::string>();
              }
            }
            break;
          case 2:
            if (curr_name != "www") {
              field_name = name;
            }
            sslc.Reset();
            break;
          default:
            if (clv == 3) {
              sslc.Append("<", name, ">");
            }
            // LOG(INFO) << "unknown name:" << name << "lv:" << clv;
        }
      })
      .OnEndElement([&](const std::string &name) {
        int clv = lv.top();
        lv.pop();
        assert(!lv.empty());
        switch (clv) {
          case 1:
            if (name != "www" && is_valid_pub) {
              pub["d_type"] = name;
              pub["author"] = author;
              LOG(INFO) << "GET!";
              
              doc["pubs"].push_back(pub);
              // SetValueByPointer(doc, "/pubs/-", pub);
            }
            break;
          case 2:
            if (curr_name != "www") {
              if (field_name == "year") {
                int year = std::atoi(sslc.ToString().c_str());
                if (year != 0) {
                  pub["year"] = year;
                }
              } else if (field_name == "author") {
                auto author_name = sslc.ToString();
                if (author_name == FLAGS_name) {
                  is_valid_pub = true;
                }
                auto fixed_author_name = NameFix(author_name);
                // if(fixed_author_name.size() != author_name.size()) {
                //   LOG(INFO) << "from: [" << author_name << "] to: [" <<
                //   fixed_author_name << "]";
                // }
                author.push_back(fixed_author_name);
              } else {
                pub[field_name] = sslc.ToString();
              }
            }
            sslc.Reset();
            break;
          default:
            if (clv == 3) {
              sslc.Append("</", name, ">");
            }
            // LOG(INFO) << "unknown name:" << name << "lv:" << clv << "buff:"
            // << sslc.ToString();
        }
      })
      .OnCharacters([&](const std::string &buff) {
        if (lv.top() > 1) {
          sslc.Append(buff);
        }
      })
      .OnWarning([&](const std::string &msg) { LOG(WARNING) << msg; })
      .OnError([&](const std::string &msg) { LOG(ERROR) << msg; });

  int status = handler.ReadFile(f, FLAGS_in.c_str());
  if (status != 0) {
    LOG(ERROR) << "xml read error" << status;
  }

  fclose(f);

  LOG(INFO) << "doc:\n" << doc.dump(4);
  LOG(INFO) << "total:" << doc["pubs"].size() << "time spending:" << m_tm.All().Ms()
            << "ms";

  {
    FILE *fout = fopen(FLAGS_out.c_str(), "wb");
    if (fout == nullptr) {
      LOG(ERROR) << "open FLAGS_out failed: " << FLAGS_out;
    } else {
      std::string dump = doc.dump(4);
      fwrite(dump.data(), sizeof(char) * dump.size(), 1, fout);
      fclose(fout);
    }
  }

  return 0;
}
