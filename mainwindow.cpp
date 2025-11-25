#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include  <random>
using namespace  std;

#include "dialogtedycja_bon_motu.h"

//string nazwa_pliku_cytatow =   "CYTATY2.TXT" ;
//string nazwa_pliku_cytatow =   "cyklotron.txt" ;
string nazwa_pliku_cytatow =   "Higgs_prompter.txt" ;

#define COTO  ; // cout << __FILE__ << " line:" << __LINE__ << endl;

// OPIS zasad tekstu
// 1. w tekscie didaskalia zaznaczamy za pomocą znaków "[--" oraz "--]"
// czyli:   zwykły tekst   [--didaskalia--] dalszy ciąg zwykłego tekstu
// 2. Imiona aktorów mają być kolorowe. wybór ich kolorów oraz wybór koloru didaskaliów
// robiony jest funkcji:     dodaj_kolorowe_kody(string txt)

//*****************************************************************************************************************
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ofstream plik_numerkow("numerki.txt");
    for(int i = 1 ; i <= 300 ; i++)
        plik_numerkow << "linia " << i << " ssss sss sssss <br>" << endl;


    // jeśli jest argument argv, to zamieniamy plik tekstowy
    if(plik_tekstowy.empty() == false)
          nazwa_pliku_cytatow = plik_tekstowy;

    font_size = 70;
    skok_recznego = 20;
    speed = 1;

    interwal = 1;
    wczytaj_opcje();
    ui->pushButton_zapis_html->setVisible(false);
    ui->label_nazwisko->setVisible(false);


    metronom = new QTimer(this);
    connect(metronom, SIGNAL(timeout()), this, SLOT(on_metronom_timeout() ) );

    ui->spinBox_rozmiaru_liter->setValue(font_size);
    ui->horizontal_slider_rozmiar_linii->setValue(font_size);

    ui->spinBox_skoku_przewijania_recznego->setValue(skok_recznego);
    ui->slider_skoku->setValue(skok_recznego);

    ui->spinBox_speed->setValue(speed);
    ui->slider_speed->setValue(speed);

    // może to poniżej nie jest potrzebne
//    ui->slider_interwalu->setVisible(false);
//    ui->spinBox_interwal->setVisible(false);
//    ui->label_interwalu->setVisible(false);
   // ui->horizontalSpacer_interwalu->setGeometry(QRect(0,0,5,5) );

    wpisz_caly_text_do_okna();

    przewijak = ui->textEdit->verticalScrollBar();


    przewijak->setRange(scroll_min, scroll_max);
     przewijak->setMaximum(scroll_max);
    przewijak->setValue(scroll_value);

    cout << "po zaladowaniu tekstu przewijak min = "
         << przewijak->minimum()
         << ", max= " << przewijak->maximum()
         << endl;
//    skrolowanie_do_pozycji(scroll_value);

    // on_metronom_timeout();
   // interwal = 1;
   metronom->start(interwal);  // wyglada na to ze dopiero to sprawi
   // że się poprawnie skroluje do nowej pzycji


    cout << "po korekcie przewijak min = "
         << przewijak->minimum()
         << ", max= " << przewijak->maximum()
         << ", value= " << przewijak->value()
         << endl;


  //  connect(metronom, SIGNAL(timeout()), this, SLOT(on_metronom_timeout() ) );


//    connect(przewijak,
//            ( QScrollBar::scro),
//            this, SLOT(on_horizontalSlider_sliderMoved(1))  );
   // fileter
//    przewijak->EventFilter()

//     connect(przewijak, SIGNAL(wheelEvent(QWheelEvent *e)) , this, SLOT(on_moje_skrolowanie(QWheelEvent *e) ) );
//      connect(ui->textEdit, SIGNAL(wheelEvent(QWheelEvent *e)) , this, SLOT(on_moje_skrolowanie(QWheelEvent *e) ) );
}
//*****************************************************************************************************************
MainWindow::~MainWindow()
{
   zapisz_opcje();
    delete ui;
}
//*****************************************************************************************************************
void MainWindow::zapisz_opcje()
{
    // zapisanie na dysku ustawień
    // rozmiar czcionki
    // font_size = 70;
    // speed = 1;
    // interwal

    ofstream plik_opcji(nazwa_pliku_opcji);
    plik_opcji
            << font_size  << "\n"
            << speed << "\n"
            << interwal << "\n"
                << scroll_min << "\n"     // scroll min
                    << scroll_max << "\n"  // scroll max
                        <<  scroll_value << "\n"  // scroll value
               ;
    plik_opcji.close();

}
//*****************************************************************************************************************
void MainWindow::wczytaj_opcje()
{




    ifstream plik_opcji(nazwa_pliku_opcji);
    if(!plik_opcji) return;

    plik_opcji
            >> font_size
            >> speed
            >> interwal
            >>  scroll_min
            >> scroll_max
            >> scroll_value;

    plik_opcji.close();

    ui->spinBox_interwal->setValue(interwal);
    ui->slider_interwalu->setValue(interwal);
}



//*****************************************************************************************************************
void MainWindow::wpisz_caly_text_do_okna()
{
    //  ui-> label_maksyma->setText( "XYZ");

    int zasadniczy_kolor_tla = 0;
    losuj_kolor();
    //    cout << "wylosowany kolor =  "
    //         << hex
    //         << zasadniczy_kolor << endl;
    int r = (zasadniczy_kolor_tla & 0xff0000) >> 16;
    int g = (zasadniczy_kolor_tla & 0x00ff00) >> 8;
    int b = (zasadniczy_kolor_tla & 0x0000ff);

    //    cout << dec << "r = " << r << ", g = " << g << ", b=" << b << endl;


    string lewy_rog = to_string(r) + ", " +  to_string(g) + ", " +  to_string(r)  ;
    //    cout << "kolor lewy_rog =  " << lewy_rog << endl;

    string prawy_rog =  to_string(r* 0.8) + ", " +  to_string(g * 0.8) + ", " +  to_string(r * 0.8);

    //    cout << "kolor prawy_rog =  " << prawy_rog
    //         << ", bo "
    //         << (r/2) << ", "
    //         << (g/2) << ", "
    //         << (b/2)
    //         <<  endl;

    ostringstream  sss;

    sss.seekp(0) ;
    sss.fill('0');
    sss << hex << setw(2) << r/8 << setw(2) << g/8 << setw(2) << b/8 ;
    kolor_textu = sss.str();
    kolor_textu = "ffffff";

    // cout << "kolor textu =  " << kolor_textu << endl;

    ui->textEdit->setStyleSheet(
                ("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, \
                 stop:0 rgba(" +
                             lewy_rog +
                             ", 255), \
                             stop:1 rgba(" +
                                         prawy_rog +
                                         ", 255));").c_str()
                             );

                para = losuj_cytat();


            //            string kolorowa_mysl = R"*(<html><head/><body><p><span style=" font-size:12pt; color:#)*"
            //            + kolor_textu
            //            + R"*(">)*"
            //            +
            //            para.first
            //            +
            //            R"*(</span></p></body></html>)*";

            //    ui-> label_maksyma->setText(kolorowa_mysl.c_str() );
            ui-> textEdit ->setText(
                zrob_kolorowa_mysl(para.first )
                );


    string kolorowe_nazwisko = R"*(<html><head/><body><p><span style=" font-family: Times, "Times New Roman", serif; font-size:16pt; color:#0000ff;">)*"
            +
            to_string(ktory_cytat) + "  "
            +
            para.second
            +
            R"*(</span></p></body></html>)*";

    ui-> label_nazwisko->setText( kolorowe_nazwisko.c_str() );
    //    ui-> label_nazwisko->setText( "ABC" );
    //    ui-> label_maksyma->adjustSize();
    //    ui->adjustSize();

    on_textEdit_selectionChanged();
    //ui->pushButton_blad->setEnabled(false) ;

   // cout << "text wypisz - max = " <<
//    ui->textEdit->verticalScrollBar()->maximum()  << endl;

}


//*****************************************************************************************************************
pair<std::string, string>  MainWindow::losuj_cytat()
{

    //	char nazwa[] = "D:\\usr\\jur\\cytat_win\\cytaty.txt" ;
    string nazwa =nazwa_pliku_cytatow;


    // otwarcie pliku z cytatami
    ifstream plik(nazwa);

    /* otwarcie zbioru */
    if(!plik)
    {
        cout << "Nie znajduje zbioru " << nazwa << endl ;
        return pair<string, string>("nieudane otwarcie pliku " + nazwa,
                                    "otwarcie zbioru") ;

    }

    zestaw.clear();

    int ile_mysli = 0;
    for(int nr = 0 ;  ; ++nr)
    {
        // przeczytanie cytatów, z których każdy rozpoczyna gwiazdka
        string zlota_mysl;
        getline(plik, zlota_mysl, '*');   // aż DO gwiazdki
        if(!plik) break;

        if(zlota_mysl.empty() == false)
        {
            while(zlota_mysl.size() )
            {
                auto znak = zlota_mysl[zlota_mysl.size() -1] ;
                if(znak == ' ' || znak == '\t' || znak == '\n' )
                {
                    zlota_mysl.erase(zlota_mysl.size() -1);
                }
                else break;
            }

            if(zlota_mysl.empty() == false)
            {
                 zlota_mysl = dodaj_kolorowe_kody(zlota_mysl);
                zestaw.push_back(zlota_mysl);
                //cout << "Wczytana mysl: [[ " << zlota_mysl << "]]" << endl;
            }
        }else{

            //ile_mysli = nr;
            break;
        }

    }

    plik.close();

    ile_mysli = zestaw.size();
    //    cout << "------------- Wczytane " << ile_mysli << " mysli " << endl;


    //----------------

    //    for(int i = 0 ; i < zestaw.size() ;++i)
    //    {
    //        cout << i << ") [" << zestaw[i] << "] ---" << endl;
    //    }

    //    cout << "koniec zestawu " << endl;

    // create default engine as source of randomness
    //std::default_random_engine dre(time(0) );
    std::mt19937_64 dre(time(0) );

    // use engine to generate integral numbers between 10 and 20 (both included)

    int zakres = ile_mysli;

    std::uniform_int_distribution<int> di(0, zakres-1);

    //    for (int i=0; i<22; ++i)
    //    {
    //        std::cout << i << ", -> "
    //        << di(dre)      // 22 razy
    //        << " ";
    //    }

    ktory_cytat = di(dre);

    int zaczac_od = 30;  // 0   // Dotąd zrobiłem korektę

    static int stat_i = zaczac_od;
    ktory_cytat = stat_i++;
    ktory_cytat = 0;    // Sztucznie dla promptera

    if(ktory_cytat >= ile_mysli) {
        cout << "Za duzy numer cytatu " << ktory_cytat
             << " jest >= " << ile_mysli << endl;
        exit(22);
    }

    // oddzielenie mysli od nazwiska
    string  mysl = zestaw[ktory_cytat];

    string nazwisko;


    //    cout << "mysl nr " << ktory_cytat << " z " << ile_mysli << " -> " ;
    //    cout <<  mysl  << endl;



    auto gdzie = mysl.find("\t\t");
    if(gdzie != string::npos)
    {
        nazwisko = mysl.substr(gdzie);
        mysl = mysl.substr(0, gdzie);

    }

    else if( (gdzie = mysl.find("#")) != string::npos)
    {
        nazwisko = mysl.substr(gdzie);
        mysl = mysl.substr(0, gdzie);

    }
    else {
        cout << "Brak hash w cytacie nr " << ktory_cytat << ": [" << mysl << "]" << endl;
        // wobec tego wyjęcie ostatiej linijki

        gdzie = mysl.rfind('\n');
        if(gdzie != string::npos){
            nazwisko = mysl.substr(gdzie);
            mysl = mysl.substr(0, gdzie) + "<br>#" + nazwisko;
        }
        // exit(2);
    }

    //    cout << "Mysl = [" << mysl << "]" << endl;
    //    cout << "Nazwisko = [" << nazwisko << "]" << endl;
    return pair<string, string>(mysl,nazwisko);

}
//********************************************************************************************************

int MainWindow::losuj_kolor()
{
    // 770000

    std::mt19937_64 dr_engine(time(0) );

    // use engine to generate integral numbers between 10 and 20 (both included)
    int zakres = 0xff;
    std::uniform_int_distribution<int> distribition(0.75 * zakres,  zakres);

    int kolor = (distribition(dr_engine) << 16) + (distribition(dr_engine) << 8) + (distribition(dr_engine));
    return   kolor;
}

//********************************************************************************************************
void MainWindow::on_buttonBox_rejected()
{
    close();
}
#if 0
//********************************************************************************************************
void MainWindow::on_pushButton_ponownie_clicked()
{
    wypisz();
}
//********************************************************************************************************


void MainWindow::on_pushButton_blad_clicked()
{

    cout << __func__  << " FUNKCJA ZABLOKOWANA" << endl;

    return;

    string plain = zamiana_boldow_na_B() ;  // to zmienic !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    cout << " Po powrocie z odczytania boldów "  << endl;
    cout << "PLAIN=" << plain << endl;

    //    string     txt_html =  ui-> textEdit -> toHtml().toStdString();
    //    cout << "A) " << txt_html << endl;

    plain = "abc";

    auto where = string::npos;
    // zmiana karetek na <br>
    for( int i = 0 ;  ; i++ )
    {
        where = plain.rfind("\n", where);
        if(where == string::npos) break;
        plain.insert(where, "<br>");

        //        cout << i << plain << endl;
    }


    //    cout << "B) " << plain << endl;
    // otwarcie pliku tekstowego do dopisywania
    ofstream plik ("bledna.txt", ios::app);
    // Zapis bon_mota
    plik << "*" <<  plain    // para.first
         << "\n\t\t#"
         << para.second
         << endl;
    // zapis autora
    // ui->pushButton_blad->setEnabled(false) ;

#if 1
    // podmiana całego pliku ---------------------------------
    ofstream plik_wy(nazwa_pliku_cytatow);
    // poprzednie -----------------
    for(int i = 0 ; i < ktory_cytat ; ++i)
    {
        plik_wy << zestaw[i] << "*\n" ;;
    }
    // wlasnie edytowany------------
    plik_wy
            <<  plain    // para.first
             << "\n"
                ;
    //if(para.first.find(para.second) ==  string::npos)      // jeśli już nie ma

    if(para.first.find('#') ==  string::npos)      // jeśli już nie ma
    {        plik_wy // << "Nie bylo tego: "
                << para.second ; // to dopisujemy nazwisko

    }
    plik_wy    << "\n*\n" ;
    // nastepne ---------------
    for(uint i =ktory_cytat + 1 ; i < zestaw.size() ; ++i)
    {
        plik_wy << zestaw[i]  << "\n*" << endl;
    }

    //-----------------------------------------

#endif



}
#endif
//********************************************************************************************************
#if 0
void MainWindow::on_pushButton_bold_clicked()
{
    zamiana_na_bold(true);
    ui->pushButton_blad->setEnabled(true) ;
}
#endif
//************************************************************************************************************
void MainWindow::on_textEdit_selectionChanged()
{
   // cout << __PRETTY_FUNCTION__ << endl;
    //    auto sel = ui-> textEdit->textCursor();
    //    int pocz = sel.selectionStart();
    //    int kon = sel.selectionEnd();

    // ui->pushButton_bold->setEnabled(pocz != kon);


}
//************************************************************************************************************
void MainWindow::on_textEdit_textChanged()
{
   // cout << __PRETTY_FUNCTION__ << endl;
    //ui->pushButton_blad->setEnabled(true) ;
    // ui->pushButton_bold->setEnabled(true) ;
}
//************************************************************************************************************
// Zdejmujemy tekst z pola dialogowego i rozpoznajemy w nim boldy
string MainWindow::zamiana_boldow_na_B()
{
    cout << __func__ << "----------------------------" << endl ;
    bool flaga_tryb_bold = false;
    auto sel = ui-> textEdit->textCursor();
    sel.select(QTextCursor::Document);
    // auto ekran =         on_pushButton_zdejm_z_ekranu_clicked();
    string zdanie_plain =
            //ekran.toStdString();
            sel.selectedText().toStdString();
    cout << " --------------po zdjeciu z ekranu = " ;
    cout << zdanie_plain << endl;

    sel.movePosition(QTextCursor::Start);

    string zdanie = zdanie_plain;
    string zdanie_uzupelnione;

    // odczytywanie BOLDów ++++++++++++++++++++++++++++++++
    for(int i = 0 ;  ; ++i){
        // cout << i << " weight  = " << sel.charFormat().fontWeight() << endl;
        if(sel.charFormat().fontWeight() == QFont::Bold){
            // cout << i << " tlusty druk " << endl;

            if( flaga_tryb_bold == false)  // dotychczas nie było bold, to ustawiamy
            {
                // zdanie_uzupelnione.push_back('^');
                auto ost =  zdanie_uzupelnione.size();

                int gdzie = ost-1;
                if(gdzie < 0) gdzie = 0 ;
                zdanie_uzupelnione. insert(gdzie, "<b>");
                flaga_tryb_bold = true;
            }

            zdanie_uzupelnione.push_back(zdanie_plain[i]);

            zdanie[i] = 'B';
        }
        else
        {
            //            cout << i << " zwykly " << endl;

            if( flaga_tryb_bold == true )  // dotychczas było bold, to zerujemy
            {
                // zdanie_uzupelnione.push_back('$');
                auto ost =  zdanie_uzupelnione.size();
                zdanie_uzupelnione. insert(ost-1, "</b>");

                flaga_tryb_bold = false;
            }

            zdanie_uzupelnione.push_back(zdanie_plain[i]);
            // zdanie.push_back('-');
        }

        if(sel.movePosition(QTextCursor::NextCharacter) == false) break;
    }
    // +++++++++++++++++++++++++++++++++++++++++++++
    cout << zdanie << "miejsca bold" << endl;
    cout << zdanie_plain << " org. text" << endl;
    cout << zdanie_uzupelnione << " zdanie_uzupelnione " << endl;
    // zamiana karetek na <br>


    vector < pair<string, string> > skasowanie {
        {"<b></b>", ""},
        {"</b><b>", ""},
        {"<b><b>", "<b>"},
        {"</b></b>", "</b>"} // ,
        //        {"\n", "<br>"},
        //        { "â€©", "<br>"}
    };
    for(uint i = 0 ; i < skasowanie.size() ; ++i)
    {
        string::size_type gdzie = 0;
        while(1){
            gdzie = zdanie_uzupelnione.find(skasowanie[i].first, gdzie );
            if(gdzie == string::npos) break;
            zdanie_uzupelnione.replace(gdzie, skasowanie[i].first.size(), skasowanie[i].second);
        }

    }
    cout << zdanie_uzupelnione << " zdanie_uzupelnione po poprawce" << endl;


    //  ui-> textEdit ->setHtml( zdanie_uzupelnione.c_str() );

    //    ui-> textEdit ->setText(
    //                zrob_kolorowa_mysl(zdanie_uzupelnione.c_str() )
    //                );
    return zdanie_uzupelnione;
}
//************************************************************************************
QString MainWindow::zrob_kolorowa_mysl(string co)
{

    int size = ui->spinBox_rozmiaru_liter->value();
    // font-family: Times, "Times New Roman", serif;

    string kolorowa_mysl = // R"*(<html><head/><body><p><span style=" font-size:)*"
            R"*(<html><head/><body><p><span style=" font-family: Times, 'Times New Roman', serif; font-size:)*"
            + to_string(size)
            + string("pt; color:#")
            + kolor_textu
            + R"*("><center>)*"
            +
            co
            +
            R"*(</center></span></p></body></html>)*";
    return  kolorowa_mysl.c_str();
}
//************************************************************************************************************
#if 0

void MainWindow::on_pushButton_unbold_clicked()
{
    cout << __func__ << endl;
    zamiana_na_bold(false);
    ui->pushButton_blad->setEnabled(true) ;
}
#endif
//************************************************************************************************************
// NOTE: uruchamiany albo gdy ktoś kliknął BOLD lub gdy kliknął UNBOLD
#if 0

void MainWindow::zamiana_na_bold(bool na_bold)
{


    bool flag_docelowo_tlusty = na_bold;

    // zapamiętanie selekcji
    auto sel = ui-> textEdit->textCursor();   // biezacy oszar

    uint pocz = sel.selectionStart();
    uint kon = sel.selectionEnd();

    // jednak przebiegniemy po całym tekscie

    QString zdanie_html = zdejm_z_ekranu(false);


    // przebiegnij przez tekst pomijając znaki sterujące
    int nr_znaku = 0;
    for(int = 0 ; i < zdanie_html.size() ; ++i)
    {
        if(zdanie_html.substr(s, <b>))

    }


    sel.select(QTextCursor::Document);
    QString zdanie_plain = sel.selectedText();
    string szdanie_plain = sel.selectedText().toStdString();
    //    string popr = zamien_wszystkie_na(zdanie_plain, "\n" , "<K>" );
    //    cout << "popr = " << popr << endl;
    sel.movePosition(QTextCursor::Start);

    string pattern ; //  = zdanie_plain;  // zeby zgadzala sie liczba znakow
    string zdanie_edyt;  // na rezultat

    cout << "Przed akcją zdanie_plain jest takie" << endl;

    for(int i = 0 ; ; ++i)// przebieganie poszczególnych znaków  ..........................................................
    {
        int znak = i ;
        //        cout << " znak  " << i
        //             << "[" << zdanie_plain[i] << "]"
        //             << " weight  = " << sel.charFormat().fontWeight() << endl;
        // if (i > 0) znak = i -1;

        if(sel.charFormat().fontWeight() == QFont::Bold )
        {
            pattern.push_back('B');
        }  else{
            //pattern.push_back('N');
            pattern.push_back(char('0' + i));
        }


        if(sel.movePosition(QTextCursor::NextCharacter) == false) break;
        // cout << "znak = " << znak << endl;
    } // koniec skanowania znaków

    cout << pattern << ", 1 pierwotny pattern, przed modyfikacja, ma znakow " << pattern.size() << endl;



    //    zdanie_plain = zamien_wszystkie_na(zdanie_plain, "â€©",  "<KARETKA>" );

    //    cout << zdanie_plain << " 3 zdanie_plain po karetkach" << endl;


    for(uint i = 0 ; i < pattern.size() ; ++i)
    {
        if  (i >= pocz && i < kon){
            //flaga_zmieniany_obszar = true;
            pattern[i]  = flag_docelowo_tlusty ? 'B' : 'N';
        }
    }
    cout << pattern << ", 4 pattern, po bieżącej modyfikacji , ma znakow " << pattern.size() << endl;

    // przepisywanie- dodające karetki i znaki ---------------------------------------

    for(uint i = 0 ; i < pattern.size() ; ++i)
    {
        auto znak = QString(zdanie_plain[i]).toStdString() ;
        cout << i << ") znak = " << znak << endl;

        if( (zdanie_plain[i] == 8233) || (zdanie_plain[i] == 8232) )
        {
            //   cout <<  "karetka " << endl;
            zdanie_edyt.append("<br>");
            continue;
        }




        if(pattern[i] == 'N')
        {
            zdanie_edyt.append(znak);
        }
        else     // pattern znak == "B'
        {
            zdanie_edyt.append(
                        string("<b>") + znak + "</b>"
                        );
        }  // kon bold



    }
    // cout << zdanie_edyt << " zdanie_edyt " << endl;



    // końcowa kosmetyka ----------------------------------------

    vector < pair<string, string> > skasowanie {
        {"<b></b>", ""},
        {"</b><b>", ""},
        {"<b><b>", "<b>"},
        {"</b></b>", "</b>"}
    };

    for(uint i = 0 ; i < skasowanie.size() ; ++i)
    {
        string::size_type gdzie = 0;
        while(1){
            gdzie = zdanie_edyt.find(skasowanie[i].first, gdzie );
            if(gdzie == string::npos) break;
            zdanie_edyt.replace(gdzie, skasowanie[i].first.size(), skasowanie[i].second);
        }
    }
    // cout << zdanie_edyt << " zdanie_uzupelnione po poprawce" << endl;

    ui-> textEdit ->setText(
                zrob_kolorowa_mysl(zdanie_edyt.c_str() )
                );

    return  ;
}
#endif
//****************************************************************************************************************************
string MainWindow::zamien_wszystkie_na(string txt,string co, string na)
{
    //    vector < pair<string, string> > skasowanie {
    //        {"<b></b>", ""},
    //        {"</b><b>", ""},
    //        {"<b><b>", "<b>"},
    //        {"</b></b>", "</b>"},
    //        {"â€©", "<br>"},
    //        {"\r", "<br>"}
    //    };


    string::size_type gdzie = 0;
    while(1)
    {
        gdzie = txt.find(co, gdzie );

        if(gdzie == string::npos) break;
        txt.replace(gdzie, co.size(), na);
    }
    return  txt;

}
////**********************************************************************************************************************************
//string MainWindow::zbuduj_pattern_tlustego_druku(string txt)
//{

//    return "BBBNNN";   // fake
//}

//**********************************************************************************************************************************
// Rozpoznanie znakow nowej linii
QString MainWindow::zdejm_z_ekranu_clicked()
{
    // cout << __func__ << endl;
    return  zdejm_z_ekranu(false ) ;    //  flag_karetki
}


//*************************************************************************************************************
QString MainWindow::zdejm_z_ekranu(bool flag_karetki)
{
    // zapamiętanie selekcji
    auto sel = ui-> textEdit->textCursor();   // biezacy oszar
//    auto  pole_tekstu = ui-> textEdit;

    // jednak przebiegniemy po całym tekscie----------------------------------------
    sel.select(QTextCursor::Document);
    QString text_html = ui->textEdit->toHtml();

    QString zdanie_plain =  sel.selectedText();
    QString wynik;
    //    cout << "plain = ma dlugosc "
    //         << zdanie_plain.size()
    //         << endl;
    sel.movePosition(QTextCursor::Start);
    bool flaga_tryb_bold = false;

    // 8233   // podmiana dwojakiego rodzaju karetek
    for(int i = 0 ; i < zdanie_plain.size() ; ++i)
    {
        //               cout << i << ") znak  [";
        //               cout <<  (char) (zdanie_plain[i]) ;
        //               cout << "] " ;
        //               wcout << "  kod = ";
        //               cout << int (zdanie_plain[i] ) << endl;
        // cout << i << ") = "<< wynik.toStdString() << endl;


        if(sel.charFormat().fontWeight() == QFont::Bold){
            // cout << i << " tlusty druk " << endl;

            if( flaga_tryb_bold == false)  // dotychczas nie było bold, to ustawiamy
            {
                // zdanie_uzupelnione.push_back('^');
                auto ost =  wynik.size();

                int gdzie = ost-1;
                if(gdzie < 0) gdzie = 0 ;
                wynik. insert(gdzie, "<b>");
                flaga_tryb_bold = true;
            }

            // wynik.push_back(zdanie_plain[i]);

            //                     zdanie[i] = 'B';
        }
        else
        {
            //  cout << i << " zwykly " << endl;

            if( flaga_tryb_bold == true )  // dotychczas było bold, to zerujemy
            {
                // zdanie_uzupelnione.push_back('$');
                auto ost =  wynik.size();
                wynik. insert(ost-1, "</b>");

                flaga_tryb_bold = false;
            }

            // wynik.push_back(zdanie_plain[i]);
            // zdanie.push_back('-');
        }

        // Karetki robimy niezależnie od tłustego druku----------------
        // Ale tylko pod warunkiem pisania na dysku

        if(zdanie_plain[i] == (QChar) 8233 && flag_karetki) {
            cout << "Zamiana 8233 na karetke " << endl;
            zdanie_plain[i]  = '\n';
            wynik.append("<br>\n");
        }else if(zdanie_plain[i] == (QChar) 8232 && flag_karetki){
            cout << "Zamiana 8232 na karetke " << endl;
            zdanie_plain[i]  = '\n';
            wynik.append("<br>\n");
        }else
            wynik.push_back(zdanie_plain[i]);

        if(sel.movePosition(QTextCursor::NextCharacter) == false) break;
    }


    // cout << "Na koniec funkcji " << __func__ << "\n plain = "   << wynik.toStdString() << endl;
    return wynik;
}
//*************************************************************************************
//*************************************************************************************************************
QString MainWindow::zdejm_z_ekranu_stary(bool flag_karetki)
{
    // zapamiętanie selekcji
    auto sel = ui-> textEdit->textCursor();   // biezacy oszar
//    auto  pole_tekstu = ui-> textEdit;

    // jednak przebiegniemy po całym tekscie----------------------------------------
    sel.select(QTextCursor::Document);
    QString text_html = ui->textEdit->toHtml();

    QString zdanie_plain = sel.selectedText();
    QString wynik;
    //    cout << "plain = ma dlugosc "
    //         << zdanie_plain.size()
    //         << endl;
    sel.movePosition(QTextCursor::Start);
    bool flaga_tryb_bold = false;

    // 8233   // podmiana dwojakiego rodzaju karetek
    for(int i = 0 ; i < zdanie_plain.size() ; ++i)
    {
        //               cout << i << ") znak  [";
        //               cout <<  (char) (zdanie_plain[i]) ;
        //               cout << "] " ;
        //               wcout << "  kod = ";
        //               cout << int (zdanie_plain[i] ) << endl;
        // cout << i << ") = "<< wynik.toStdString() << endl;


        if(sel.charFormat().fontWeight() == QFont::Bold){
            // cout << i << " tlusty druk " << endl;

            if( flaga_tryb_bold == false)  // dotychczas nie było bold, to ustawiamy
            {
                // zdanie_uzupelnione.push_back('^');
                auto ost =  wynik.size();

                int gdzie = ost-1;
                if(gdzie < 0) gdzie = 0 ;
                wynik. insert(gdzie, "<b>");
                flaga_tryb_bold = true;
            }

            // wynik.push_back(zdanie_plain[i]);

            //                     zdanie[i] = 'B';
        }
        else
        {
            //  cout << i << " zwykly " << endl;

            if( flaga_tryb_bold == true )  // dotychczas było bold, to zerujemy
            {
                // zdanie_uzupelnione.push_back('$');
                auto ost =  wynik.size();
                wynik. insert(ost-1, "</b>");

                flaga_tryb_bold = false;
            }

            // wynik.push_back(zdanie_plain[i]);
            // zdanie.push_back('-');
        }

        // Karetki robimy niezależnie od tłustego druku----------------
        // Ale tylko pod warunkiem pisania na dysku

        if(zdanie_plain[i] == (QChar) 8233 && flag_karetki) {
            cout << "Zamiana 8233 na karetke " << endl;
            zdanie_plain[i]  = '\n';
            wynik.append("<br>\n");
        }else if(zdanie_plain[i] == (QChar) 8232 && flag_karetki){
            cout << "Zamiana 8232 na karetke " << endl;
            zdanie_plain[i]  = '\n';
            wynik.append("<br>\n");
        }else
            wynik.push_back(zdanie_plain[i]);

        if(sel.movePosition(QTextCursor::NextCharacter) == false) break;
    }


    // cout << "Na koniec funkcji " << __func__ << "\n plain = "   << wynik.toStdString() << endl;
    return wynik;
}
//*************************************************************************************
void MainWindow::zapis_calosci_na_dysku()
{
    // obecna wersja jest w para.first, para.second

    // podmiana całego pliku ---------------------------------
    ofstream plik_wy(nazwa_pliku_cytatow);
    // poprzednie -----------------
    for(int i = 0 ; i < ktory_cytat ; ++i)
    {
        plik_wy << zestaw[i] << "\n*" ;;
    }
    // wlasnie edytowany------------
    plik_wy
            <<  para.first
             << "\n"
                ;
    //if(para.first.find(para.second) ==  string::npos)      // jeśli już nie ma

    if(para.first.find('#') ==  string::npos)      // jeśli już nie ma
    {        plik_wy // << "Nie bylo tego: "
                << para.second ; // to dopisujemy nazwisko

    }
    plik_wy    << "\n*" ;
    // nastepne ---------------
    for(uint i =ktory_cytat + 1 ; i < zestaw.size() ; ++i)
    {
        plik_wy << zestaw[i]  << "\n*" ; // << endl;
    }

    //-----------------------------------------


}
//*************************************************************************************
void MainWindow::on_pushButton_do_edycji_clicked()
{
    DialogTedycja_bon_motu dlg;

    auto txt = zdejm_z_ekranu_clicked();


    auto text_ht =  txt ; // zamiana_boldow_na_B();

    //    cout << __func__
    //         << " rezultat po doczytaniu boldow = "
    //         << text_ht.toStdString() << endl;


    // zamiana tłustego druku na kody sterujące

    // min i max są pozycjami samego suwaka, czyli widocznej części tekstu
    // a nie jego całego slitera !
    scroll_min = przewijak->minimum(); // NOWE

    scroll_max = przewijak->maximum();
    scroll_value = przewijak->value();

   int poz1 = ui->textEdit->textCursor().position();
   cout << "Pierwotna pozycja kursora = " << poz1 << endl;
   int rob = scroll_value - (przewijak->pageStep())/2;
   if (rob < 0) rob = 0;
    dlg.do_edycji(text_ht.toStdString(), scroll_min, scroll_max,
                  rob, font_size);
    // moze tutaj ustawic pozycje zdjętą z obecnej pozycji promptera


    switch(dlg.exec())
    {
    case QDialog::Accepted:
    {
        auto rezultat =  dlg.give_edited().toStdString();
        para.first = rezultat;

        rezultat = dodaj_kolorowe_kody(rezultat);

        ui->textEdit->setText( zrob_kolorowa_mysl(rezultat) );
        zapis_calosci_na_dysku();             
        break;
    }

    case QDialog::Rejected:
        break;


    }
    int val = 0; int koniec = 10;
    dlg.daj_pozycje_skrollera(& val, & koniec);

   cout << "przyszły dane val = " << val << ", koniec = " << koniec << endl;


    scroll_value = val *  przewijak->maximum() / koniec;

    skrolowanie_do_pozycji(scroll_value);
//    cout
//            << "POzycjonowanie do wartosci "
//            << scroll_value
//            << endl;

    flag_poczatkowo_pozycjonuj = true;
     metronom->start(interwal);  // wyglada na to ze dopiero to sprawi
    // że się poprawnie skroluje do nowej pzycji


}
//*************************************************************************************
//void MainWindow::on_spinBox_valueChanged(int arg1)
//{
//    ui->horizontal_slider_rozmiar_linii->setValue(arg1);
//    cout << __PRETTY_FUNCTION__ << endl;
//    wypisz();
//}
//*************************************************************************************
//void MainWindow::on_horizontalSlider_sliderMoved(int position)
//{
//    ui->spinBox_rozmiaru_liter->setValue(position);
//    cout << __PRETTY_FUNCTION__ << endl;
//     wypisz();
//}
//**************************************************************************************
void MainWindow::wheelEvent(QWheelEvent *e)
{

    // Tu jesteśmy gdy myszka jest na "sterującym" polu dialogowym, ale nie na skrollerach
    // tylko w jakimś neutralnym miejscu

//cout << __PRETTY_FUNCTION__ << endl;
    auto  delta =   ( e->angleDelta()/ 60);

    cout << "wheelEvent, delta = " <<    delta.y()  << endl;

//    if(flaga_automatycznie){
//        cout << " tryb automatyczny" << endl;
//        speed = (delta < 0) ? (speed +1) : (speed -1) ;
//        ui->spinBox_speed->setValue(speed);
//        ui->slider_speed->setValue(speed);
//    }
//    else {
//        cout << " tryb reczny " << endl;

//        delta = delta>0 ? skok_recznego : -skok_recznego;

//        cout << "wobec tego skok = " << delta << endl;
//        skrolowanie_tekstu(delta);
//    }
}
//*************************************************************************************

//*************************************************************************************
void MainWindow::on_metronom_timeout()
{
     //cout << __PRETTY_FUNCTION__ << endl;

    if(flag_poczatkowo_pozycjonuj){
         skrolowanie_do_pozycji(scroll_value);
         flag_poczatkowo_pozycjonuj = false;
         return;
    }



    // Żeby przyspieszyć skrolowanie można
    // dać krótszy interwał ticków lub zwiększyć speed

    // żeby zwolnić skrolowanie można
    // dać dłuższy interwał (ale wtedy obraz może skakać)
    // lub zmiejszyć speed.

    //skrolowanie_do_pozycji(29000);

    if(skrolowanie_tekstu(-speed) )
    {
        metronom->start(interwal);
    }
}

//void MainWindow::on_moje_skrolowanie(QWheelEvent *e) {
//cout << __PRETTY_FUNCTION__ << endl;
//}


//*************************************************************************************
void MainWindow::skrolowanie_do_pozycji(int poz)
{
            ui->textEdit-> verticalScrollBar()->setValue(poz);
         // cout << "skrolowanie_tekstu do pozycji " << poz << endl;

    on_pushButton_toggled(true);
    on_pushButton_toggled(false);

}
//*************************************************************************************
bool MainWindow::skrolowanie_tekstu(int delta)
{
    auto * sb = ui->textEdit-> verticalScrollBar();
    //sb->max

    scroll_value = sb->value();

    scroll_value += -delta;

   sb->setValue(scroll_value);

   // sb->triggerAction(QAbstractSlider::SliderSingleStepAdd);

    scroll_max = sb->maximum();
    scroll_min = sb->minimum();

//         cout
                string wypis =  ", pozycja tekstu zmienia sie na taka = "
                        + to_string(sb->value()) +
                        ", (tj. " +
                        to_string(100.0* sb->value() /sb->maximum ())  +
                 "%,  skrolowanie_tekstu o wartosc delta = " + to_string(delta) +
                ", interwal = " + to_string( interwal)+ " ms" ;

  //              << endl;

         if(scroll_value < 0 || scroll_value > sb->maximum())
         {
             // cout << "Koniec/poczatek tekstu !!!!!!!!!!!!" << endl;
             // gasimy przewijanie automatyczne
             on_pushButton_toggled(false);
             return false;
         }

        double ps = (delta * (1000.0 / interwal));

//     cout  << ",  to oznacza units na sek = " << ps << endl;
//       //  ui->spin_pix_na_sek->setValue(ps);
     string txt = wypis + ", units/sek =" + to_string(ps);
        ui->label_piks_na_sekunde->setText( txt.c_str() );
         return true;
}
//**************************************************************************************
void MainWindow::keyPressEvent(QKeyEvent * /* e */ )
{
    cout << "key Event" << endl;
}
//***********************************************************************************
void MainWindow::on_slider_skoku_sliderMoved(int position)
{
    skok_recznego = position;
    ui->spinBox_skoku_przewijania_recznego->setValue(position);

    ui->textEdit-> verticalScrollBar()->setSingleStep(skok_recznego);
}
////***********************************************************************************
//void MainWindow::on_spinBox_skoku_valueChanged(int arg1)
//{
//    ui->textEdit-> verticalScrollBar()->setValue(arg1);
//     skok_recznego = arg1;
//}
//***********************************************************************************
void MainWindow::on_slider_speed_sliderMoved(int position)
{
    speed = position;
    ui->spinBox_speed->setValue(speed);
}
//***********************************************************************************
void MainWindow::on_spinBox_speed_valueChanged(int arg1)
{
    speed = arg1;
    ui->slider_speed->setValue(speed);
}
//***********************************************************************************
void MainWindow::on_pushButton_toggled(bool checked)
{
    flaga_automatycznie = checked;

    ui->spinBox_skoku_przewijania_recznego->setVisible(!checked);
    ui->slider_skoku->setVisible(!checked);
    ui->label_skok_przew_recznego->setVisible(!checked);
    ui->label_rozmiar_liter->setVisible(!checked);
    ui->spinBox_rozmiaru_liter->setVisible(!checked);
    ui->horizontal_slider_rozmiar_linii->setVisible(!checked);
    ui->pushButton_do_edycji->setVisible(!checked);


    flaga_automatycznie = checked;

    if(checked) {
//        cout << "Uruchomione przewijanie glownego okna " << endl;
        metronom->start(interwal);                      // 0.1 sek
    }
    else {
//        cout << "Zatrzymane przewijanie  glownego okna " << endl;
        metronom->stop();    
    }
    ui->pushButton->setText(checked? "Pauza" :  "Start");
}
//***************************************************************************************
void MainWindow::on_slider_speed_valueChanged(int value)
{
    speed = value;
    ui->spinBox_speed->setValue(speed);
}
//***************************************************************************************
void MainWindow::on_slider_interwalu_valueChanged(int value)
{
    interwal = value;
    ui->spinBox_interwal->setValue(value);
    if(flaga_automatycznie){
        metronom->stop();
        metronom->start(interwal);
    }
}
////***************************************************************************************
//void MainWindow::on_slider_interwalu_sliderMoved(int position)
//{
//interwal = position;
//ui->
//}
//***************************************************************************************
void MainWindow::on_spin_pix_na_sek_valueChanged(int /* ps */)
{

//    for(int sp = 100 ; sp > 0 ; --sp){
//        int inter2  =  sp / (0.01 *  ps);
//        cout << "inter2 = " << inter2 << endl;

//        if(inter2 < 20)
//        {
//            interwal = inter2;
//            speed = sp;
//            cout << "zatem speed = " << speed << ", interwal = " << interwal << endl;
//            break;
//        }
//    }

    ui->spinBox_interwal->setValue(interwal);
    ui->spinBox_speed->setValue(speed);
}
//***************************************************************************************
void MainWindow::on_spinBox_rozmiaru_liter_valueChanged(int arg1)
{
    ui->horizontal_slider_rozmiar_linii->setValue(arg1);
 //   cout << __PRETTY_FUNCTION__ << endl;
    font_size = arg1;
    wpisz_caly_text_do_okna();
}
//***************************************************************************************
void MainWindow::on_horizontal_slider_rozmiar_linii_sliderMoved(int position)
{
    ui->spinBox_rozmiaru_liter->setValue(position);
//    cout << __PRETTY_FUNCTION__ << endl;
    font_size = position;
    wpisz_caly_text_do_okna();
}
//***************************************************************************************
void MainWindow::on_spinBox_skoku_przewijania_recznego_valueChanged(int arg1)
{
    ui->textEdit-> verticalScrollBar()->setValue(arg1);
    skok_recznego = arg1;
}

//void MainWindow::on_textEdit_wheelEvent(QWheelEvent *e)
//{
//cout << __PRETTY_FUNCTION__ << endl;
//}
//***************************************************************************************




//***********************************************************************************
void MainWindow::on_pushButton_zapis_html_clicked()
{
    // ui->textEdit->tex
    QString text_html = " To jest tekst JUREK:  a to KONRAD:  mowi cos [komentarz] drugi JUREK: a potem juz nic";

                        //ui->textEdit->toHtml();
//    ui->textEdit->
    // cout << "tresc = " << text_html.toStdString()  << endl;

    dodaj_kolorowe_kody(text_html.toStdString());

}
//***********************************************************************************
string MainWindow::dodaj_kolorowe_kody(string txt)
{
    // 1. Zamienić jjj
    // 2. Zamienic kkk
    // 3. zamienic ddd

    // head dodać
    string inwokacja =
    R"***(<head>
    <style>
            jjj { color: cyan; }
            kkk { color: red; }
            ddd { color: orange; }
    </style>

    </head>
    <body>
            )***";

    /// powyżej:
    ///  ddd wybór koloru dla didaskaliów
    /// jjj wybor koloru dla imienia Jurek
    /// kkk wybór koloru dla imienia Konrad

    // 1. Zamienić zamiany imion aktorów na imiona  z atrybutem koloru
    vector<string> zamiany =
    {
        "Jochym:" , "<jjj>Jochym:</jjj>",
        "Bruck:" , "<kkk>Bruck:</kkk>",
        "[--" , "<ddd>[--",
        "--]" , "--]</ddd>"
    };

   string zakonczenie = { "</body> "};

    string txtcorr = txt;
    string::size_type poz;
    //---------------------------------------------==
    cout << "size of zamiany = " << zamiany.size() << endl;
   for(uint i = 0 ; i < zamiany.size() ; i+=2 )
   {
       // zamiana jednego wyrazu
        cout << "zamiana i = " << i  << " wyrazu: " << zamiany[i] << endl;
       poz = 0;
       while(1){
           poz = txtcorr.find(zamiany[i], poz);
           if(poz == string::npos)
               break;

           cout << "bedzie zastepowanie " << zamiany[i] << endl;
           int ile_zastapic = zamiany[i].size();
          //auto nowy =
                  txtcorr.replace(poz, ile_zastapic, zamiany[i+1], 0, zamiany[i+1].size() );
           poz += zamiany[i+1].size();
       }
   }
    cout << "Po zamianach = " << txtcorr << endl;

   return inwokacja + txtcorr + zakonczenie;

}
