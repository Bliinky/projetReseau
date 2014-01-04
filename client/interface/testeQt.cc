#include <QApplication>
#include <QPushButton>

int main(int argc,char* argv[])
{
  QApplication app(argc,argv);
  QPushButton hello("Hello World!");
  hello.show();
  return app.exec();
}
