#include "main.hpp"
#include <matplot/matplot.h>

int main() {

  quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "⏰Starting at {}!", time_str);
  logger->flush_log();

  LOG_INFO(logger, "test0");
  using namespace matplot;
  std::cout << "test1" << std::endl;
  std::vector<double> x = linspace(0, 2 * pi);
  std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
  std::vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
  std::vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
  plot(x, y1, "g", x, y2, "b--o", x, y3, "c*");
  LOG_INFO(logger, "test1");
  show();
  LOG_INFO(logger, "test2");
  return 0;
}