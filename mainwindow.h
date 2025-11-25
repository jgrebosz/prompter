#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

#include<QWheelEvent>
#include <QScrollBar>
#include <QTimer>

#include <string>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern std::string plik_tekstowy ;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::pair<std::string, std::string>    losuj_cytat();
    int losuj_kolor();
    QString zdejm_z_ekranu(bool flag_karetki);
    QString zdejm_z_ekranu_stary(bool flag_karetki);
    void zapis_calosci_na_dysku();
    QString  zdejm_z_ekranu_clicked();
    std::string dodaj_kolorowe_kody(std::string txt);

private slots:
    void on_buttonBox_rejected();
   //  void on_pushButton_ponownie_clicked();
    void on_pushButton_do_edycji_clicked();
    void on_pushButton_toggled(bool checked);

    void on_textEdit_selectionChanged();
    void on_textEdit_textChanged();
    virtual void  keyPressEvent(QKeyEvent *) override;

    void on_slider_skoku_sliderMoved(int position);
    void on_slider_speed_sliderMoved(int position);
    void on_slider_speed_valueChanged(int value);

  // void  scrollContentsBy(int dx, int dy)  {}


    void on_slider_interwalu_valueChanged(int value);

    void on_spin_pix_na_sek_valueChanged(int);
//    void on_spin_pix_na_sek_textChanged(const QString &arg1);

    void on_spinBox_rozmiaru_liter_valueChanged(int arg1);
    void on_horizontal_slider_rozmiar_linii_sliderMoved(int position);
    void on_spinBox_speed_valueChanged(int arg1);
    void on_spinBox_skoku_przewijania_recznego_valueChanged(int arg1);
    void on_metronom_timeout();
   // void on_slider_interwalu_sliderMoved(int position);

    void on_pushButton_zapis_html_clicked();

protected slots:
   // void on_textEdit_wheelEvent(QWheelEvent *e);
    void wheelEvent(QWheelEvent *e) override;
 //  void on_moje_skrolowanie(QWheelEvent *e);

private:
    Ui::MainWindow *ui;

     std::vector<std::string> zestaw;
      int ktory_cytat  = 0;
    void wpisz_caly_text_do_okna();
    std::pair<std::string, std::string>  para;
    std::string zamiana_boldow_na_B();
    QString zrob_kolorowa_mysl(std::string co);
     std::string kolor_textu;
     void zamiana_na_bold(bool na_bold);
     std::string zamien_wszystkie_na(std::string txt, std::string co, std::string na);
     // std::string zbuduj_pattern_tlustego_druku(std::string txt );
      bool skrolowanie_tekstu(int delta);
      int skok_recznego;
      int speed = 1;
       int font_size = 70;
       // pozycja kursora tekstu
       int  scroll_min = 0,
       scroll_max = 10'000,
       scroll_value = 0;


    QTimer   *metronom;
    int interwal = 1; // ms   // odpowiada za płynność ruchu (widoczne przy duzych skokach!)
    bool flaga_automatycznie = false; // czy skrolujemy automatczynie czy ręcznie

    QScrollBar * przewijak = nullptr;

    std::string nazwa_pliku_opcji = "prompter_opcje.txt";
    void zapisz_opcje();
    void wczytaj_opcje();
    void skrolowanie_do_pozycji(int poz);
    bool flag_poczatkowo_pozycjonuj = true;

};
#endif // MAINWINDOW_H
