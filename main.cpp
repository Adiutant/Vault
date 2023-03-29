#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QSharedMemory>
#include <QDesktopServices>
#include <QSystemSemaphore>
#include "api/schemeeventfilter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSystemSemaphore semaphore("<uniq id>", 1);  // создаём семафор
       semaphore.acquire(); // Поднимаем семафор, запрещая другим экземплярам работать с разделяемой памятью
       QSharedMemory sharedMemory("<uniq id 2>");  // Создаём экземпляр разделяемой памяти
          bool is_running;            // переменную для проверки ууже запущенного приложения
          if (sharedMemory.attach())
          {                           // пытаемся присоединить экземпляр разделяемой памяти
                                      // к уже существующему сегменту
              is_running = true;      // Если успешно, то определяем, что уже есть запущенный экземпляр
          }
          else
          {
              sharedMemory.create(1); // В противном случае выделяем 1 байт памяти
              is_running = false;     // И определяем, что других экземпляров не запущено
          }
          semaphore.release();        // Опускаем семафор

          // Если уже запущен один экземпляр приложения, то сообщаем об этом пользователю
          // и завершаем работу текущего экземпляра приложения
          if (is_running)
          {
              QMessageBox msgBox;
              msgBox.setIcon(QMessageBox::Warning);
              msgBox.setText(QObject::tr("Приложение уже запущено.\n"
                              "Вы можете запустить только один экземпляр приложения."));
              msgBox.exec();
              return 1;
          }
    QString path = QDir::toNativeSeparators(qApp->applicationFilePath());
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);
    settings.beginGroup("Vault");
    settings.setValue("Default","URL:Vault Protocol");
    settings.setValue("DefaultIcon/Default","");
    settings.setValue("URL Protocol","");
    settings.setValue("shell/open/command/Default", QString("\"%1\"").arg(path) +"\"%1\"" );
    settings.endGroup();
    QCoreApplication::setApplicationName( QString("Vault") );

    MainWindow w;
    w.show();
    return a.exec();
}
