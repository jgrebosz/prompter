#ifndef DIALOGTEDYCJA_BON_MOTU_H
#define DIALOGTEDYCJA_BON_MOTU_H

#include <QDialog>
#include <QScrollBar>

namespace Ui {
class DialogTedycja_bon_motu;
}

class DialogTedycja_bon_motu : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTedycja_bon_motu(QWidget *parent = nullptr);
    ~DialogTedycja_bon_motu();

    void do_edycji(std::string txt, int pocz, int kon, int value, int font_size);
   QString give_edited();
   void daj_pozycje_skrollera(int *valptr, int *konptr );
private slots:
    void on_pushButton_save_edited_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_Bold_code_clicked();

    void on_pushButton_bold_finish_clicked();

    void on_pushButton_bold_clicked();

    void on_pushButton_not_bold_clicked();

    void on_plainTextEdit_edytor_selectionChanged();
    void on_timer();

    void on_pushButton_cudzyslow_clicked();

    void on_pushButton_bez_cudzyslowy_clicked();

    void on_pushButton_didaskalia_clicked();

private:
    Ui::DialogTedycja_bon_motu *ui;
    QTimer *tim;

    // dane do skrollera
    int pocz = 0 ;
    int kon = 2000;
    int value = 0;
    QScrollBar *  przewijak = nullptr;
};

#endif // DIALOGTEDYCJA_BON_MOTU_H
