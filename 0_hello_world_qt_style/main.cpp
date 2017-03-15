//
// Qt Includes
//
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char ** argv) 
{
  // Start the Application
  QApplication app(argc, argv);
  QWidget widget;
  QVBoxLayout layout;
  widget.setLayout (&layout);

  QLabel label(QStringLiteral("Hello World"), &widget);
  QLabel *label2 = new QLabel(QStringLiteral ("Goodbye World"), &widget);

  layout.addWidget(&label);
  layout.addWidget(label2);

  widget.show();

  label.show();
  // Start the Application Event Loop
  return app.exec();
}


