#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QTimer>
#include <QElapsedTimer>
#include "raylib.h"
#include "game.h"

int Main_Menu(int argc, char *argv[]){    
    
    QApplication app(argc, argv);
    
    QWidget main_menu;
    main_menu.resize(width, height);

    QLabel *background = new QLabel(&main_menu);
    QPixmap bgPixmap("Background.png");
    QPixmap scaledBg = bgPixmap.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    background->setPixmap(scaledBg);
    background->setGeometry(0, 0, width, height);
    background->lower();

    QLabel title("Game Maker!", &main_menu);
    title.setGeometry(17*width/72, 9*height/100, 1000, 200);
    QFont font = title.font();
    font.setBold(true);
    font.setUnderline(true);
    font.setPointSize(50);
    title.setFont(font);
    title.setStyleSheet("Color: red;");
    title.setAlignment(Qt::AlignCenter);

    QPushButton start_game("Start", &main_menu);
    start_game.setGeometry(19*width/36, 3*height/10, 120, 60);
    start_game.setStyleSheet("background-color: rgba(0,0,0,200); color: white; font-size: 25px; padding: 10px; border: 1px solid white");

    QPushButton info("Info", &main_menu);
    info.setGeometry(19*width/36, 2*height/5, 120, 60);
    info.setStyleSheet("background-color: black; color: white; font-size: 25px; padding: 10px; border: 1px solid white");

    QPushButton quit_game("Quit", &main_menu);
    quit_game.setGeometry(19*width/36, height/2, 120, 60);
    quit_game.setStyleSheet("background-color: black; color: white; font-size: 25px; padding: 10px; border: 1px solid white");

    QLabel infotext(&main_menu);
    infotext.setText("Welcome to Game Maker! You are Mario trying to collect all the coins to exit, but you need to build the level first! Can you build a wicked level! Left/Right arrow keys to move, space bar to jump, left mouse to build, right mouse to remove!");
    infotext.setGeometry(17*width/45, 5*height/8, 500, 300);
    infotext.setStyleSheet("color: white; font-size: 25px;");
    infotext.setAlignment(Qt::AlignCenter);
    infotext.setWordWrap(true);
    infotext.hide();
    
    QObject::connect(&info, &QPushButton::clicked, [&]() {
        infotext.setVisible(!infotext.isVisible());
    });

    QObject::connect(&quit_game, &QPushButton::clicked, [&]() {
        main_menu.close();
        app.quit();
    });

    QObject::connect(&start_game, &QPushButton::clicked, [&main_menu, &app]() {
        main_menu.hide();
        
        run_game();
        
        main_menu.show();
    });

    main_menu.show();
    main_menu.setWindowTitle(QApplication::translate("toplevel", "Game Maker!"));
    
    return app.exec();
}
