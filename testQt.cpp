#include <QApplication>
#include <QTableWidget>
#include <QComboBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTableWidget table(5, 4);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 4; ++j) {
            QComboBox* combo = new QComboBox();
            combo->addItems({"Option 1", "Option 2", "Option 3"});
            table.setCellWidget(i, j, combo);
        }
    table.show();
    return app.exec();
}


