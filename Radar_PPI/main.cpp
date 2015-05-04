
#include "radar_ppi.h"

int main(int argc, char *argv[])
{    
    Q_INIT_RESOURCE(Radar_PPI);

    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    qApp->addLibraryPath("plugins");
    
    Radar_PPI radar(30); //刷新间隔
    radar.show();


    return a.exec();
}
