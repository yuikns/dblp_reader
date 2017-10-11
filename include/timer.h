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
 **/
#ifndef APP_DBLP_READER_INCLUDE_TIMER_H_
#define APP_DBLP_READER_INCLUDE_TIMER_H_

#include <chrono>
#include <map>
#include <string>

namespace argcv {
namespace timer {

typedef std::chrono::high_resolution_clock hrclock;
typedef std::chrono::duration<double, std::milli> mil;
typedef std::chrono::duration<int64_t, std::ratio<1, 1000000000>> nal;
typedef std::chrono::time_point<hrclock, nal> time_point;

class Timer {
 public:
  Timer():start_time_porint_(hrclock::now()) {}

  ~Timer() {}

  void Label(const std::string& k) {
    time_point_label_map_.insert(std::make_pair(k, hrclock::now()));
  }

  // start to now
  double All() { return mil(hrclock::now() - start_time_porint_).count(); }

  double From(const std::string& k) {
    std::map<std::string, time_point>::const_iterator p = time_point_label_map_.find(k);
    return p == time_point_label_map_.end() ? -1.0 : mil(hrclock::now() - p->second).count();
  }

  double To(const std::string& k) {
    std::map<std::string, time_point>::const_iterator p = time_point_label_map_.find(k);
    return p == time_point_label_map_.end() ? -1.0 : mil(p->second - start_time_porint_).count();
  }

  double Between(const std::string& t, const std::string& f) {
    std::map<std::string, time_point>::const_iterator p0 = time_point_label_map_.find(t);
    std::map<std::string, time_point>::const_iterator p1 = time_point_label_map_.find(f);
    return p0 == time_point_label_map_.end() || p1 == time_point_label_map_.end()
               ? -1.0
               : (p0->second > p1->second
                      ? mil(p0->second - p1->second).count()
                      : mil(p1->second - p0->second).count());
  }

 protected:
  time_point start_time_porint_;  // start point
  std::map<std::string, time_point> time_point_label_map_;
};
}
}  // argcv::timer

#endif  //  APP_DBLP_READER_INCLUDE_TIMER_H_
