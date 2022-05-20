#pragma once
// (C) Kajetan Brzuszczak 2022

#include <chrono>
#include <deque>
#include <future>
#include <sstream>
#include <string>

#if _WIN32
#include <Windows.h>
#endif

namespace HolidayBag {

class SummaryBag {
  struct Summary {
    std::string jobName;
    std::string unit;
    int64_t duration;
    int64_t timestamp;
  };
  std::deque<SummaryBag::Summary> summaries;

public:
  void emplace(Summary &&summary) { summaries.emplace_front(summary); }

  std::string unknit() {
    std::stringstream ss("Summary:\n");
    while (!summaries.empty()) {
      auto &&summary = summaries.back();
      ss << summary.timestamp << "::" << summary.jobName << "::" << summary.duration
         << summary.unit << '\n';
      summaries.pop_back();
    }

    return ss.str();
  }
};

// Mesasures task time till end of scope, or stop() execution
// All results are transfer to shared SummaryBag, where you can read them once
// at the end. At the moment Bug is shared only between same Unit parameter.
template <typename Unit = std::chrono::microseconds> class SportTimer {
  static SummaryBag globalSummaryBag;

  std::string _name;
  std::string _unitName;
  int _factor;
  bool _isStopped;
  bool _is_paused;

  int64_t _durations_sum = 0;
  int _durations_number = 0;

#if _WIN32
  LARGE_INTEGER _start_win32, _stop_win32, _frequency_win32;
#else
  std::chrono::time_point<std::chrono::high_resolution_clock> _start, _stop;
#endif

public:
  explicit SportTimer(std::string jobName, std::string unitName = "",
                      int factor = 1)
      : _name(jobName), _unitName(unitName), _factor(factor),
        _isStopped(false) {
    if (factor < 1) {
      throw std::invalid_argument("Factor should be larger than 0");
    }
#if _WIN32
    static_assert(std::is_same<Unit, std::chrono::microseconds>::value,
                  "SportTimer supports only microseconds(us) for Windows");
    QueryPerformanceFrequency(&_frequency_win32);
    QueryPerformanceCounter(&_start_win32);
#else
    _start = std::chrono::high_resolution_clock::now();

#endif
  }

  ~SportTimer() { stop(); }

  void lap() {
    if (_isStopped) {
      return;
    }

    auto [elapsedTime, timestamp] = finish_lap();
    auto duration = elapsedTime / _factor;
    SportTimer::globalSummaryBag.emplace({_name, _unitName, duration, timestamp});
  }

  void mini_lap(bool skip = false) {
    if (_isStopped) {
      return;
    }

    auto [elapsedTime, timestamp] = finish_lap();
    if (skip) {
      return;
    }
    _durations_sum += elapsedTime;
    if (++_durations_number >= _factor) {
      _durations_number = 0;
      auto duration = _durations_sum / _factor;
      _durations_sum = _durations_sum % _factor;
      SportTimer::globalSummaryBag.emplace({_name, _unitName, duration, timestamp});
    }
  }

  void stop() {
    if (_isStopped) {
      return;
    }
    auto [elapsedTime, timestamp] = finish_lap();
    _isStopped = true;

    auto duration = elapsedTime / _factor;
    SportTimer::globalSummaryBag.emplace({_name, _unitName, duration, timestamp});
  }

  SummaryBag &getInterSummaryBag() { return globalSummaryBag; };

private:
  auto finish_lap() {
#if _WIN32
    QueryPerformanceCounter(&_stop_win32);
    auto elapsedTime = (_stop_win32.QuadPart - _start_win32.QuadPart) *
                       1000000 / _frequency_win32.QuadPart;
    _start_win32 = _stop_win32;
#else
    _stop = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<Unit>(_stop - _start).count();
    _start = _stop;
#endif
    return std::make_pair(elapsedTime, timestamp(_start));
  }

  constexpr int64_t timestamp(const std::chrono::system_clock::time_point &time_point) {
    std::chrono::system_clock::duration duration = time_point.time_since_epoch();
    int64_t timestamp = duration.count();
    return timestamp;
}
};

template <typename Unit> SummaryBag SportTimer<Unit>::globalSummaryBag;

} // namespace HolidayBag