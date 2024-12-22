
#include "main.hpp"
#include "rapidcsv.h"
#include <matplot/matplot.h>

using namespace std;

int main() {

  quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "⏰Starting at {}!", time_str);
  logger->flush_log();

  LOG_INFO(logger, "test0");

  rapidcsv::Document doc("nohdr.csv", rapidcsv::LabelParams(-1, -1));
  //  rapidcsv::SeparatorParams(';'));
  vector<float> open = doc.GetColumn<float>(0);
  cout << "Read " << open.size() << " values." << endl;
  for (auto iter : open)
    cout << "Read " << iter << " value." << endl;

  vector<float> close = doc.GetColumn<float>(5);
  cout << "Read " << close.size() << " values." << endl;
  for (auto iter : close)
    cout << "Read " << iter << " value." << endl;

  long long volume = doc.GetCell<long long>(4, 2);
  cout << "Volume " << volume << " on 2017-02-22." << endl;

  using namespace matplot;

  vector<double> x = linspace(0, 5);
  // vector<double> y1 = transform(x, [](auto x) { return sin(x); });
  // vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
  // vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
  // plot(x, y1, "g", x, y2, "b--o", x, y3, "c*");
  plot(x, open, "g", x, close, "b--o");
  show();
  return 0;
}
