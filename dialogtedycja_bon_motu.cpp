#include "dialogtedycja_bon_motu.h"
#include "ui_dialogtedycja_bon_motu.h"

#include <QAbstractSlider>
#include <QScrollBar>
#include <QTimer>
#include <iostream>
using namespace std;

//***********************************************************************************************
DialogTedycja_bon_motu::DialogTedycja_bon_motu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTedycja_bon_motu)
{
    ui->setupUi(this);

    tim = new QTimer(this);
    connect(tim, SIGNAL(timeout()), this, SLOT(on_timer() ) );

    //ui->plainTextEdit_edytor->setReadOnly(true);
     przewijak = ui->plainTextEdit_edytor->verticalScrollBar();
     ui->plainTextEdit_edytor->ensureCursorVisible();
}

//***********************************************************************************************
DialogTedycja_bon_motu::~DialogTedycja_bon_motu()
{
    delete ui;
}
//***********************************************************************************************
void DialogTedycja_bon_motu::do_edycji(std::string txt,
                                       int pocz_, int kon_, int value_, int font_size)

{
    // zamiana tlustego druku na symbole sterujace
    cout << __PRETTY_FUNCTION__<< endl;

    cout << "Do okna edycji przyszly pozycje: "
           << ", pocz = " << pocz_
           << ", kon = " << kon_
           <<    " val = " << value_
              << ", font_size " <<  font_size << endl;

     ui->plainTextEdit_edytor->setPlainText(txt.c_str());
    font_size -= 25;
    if(font_size < 15) font_size = 15;

     string s =
     R"*( QPlainTextEdit  {font-family: Times, 'Times New Roman', serif; font-size: )*"
            + to_string(font_size)
                 + string("pt; }" );

  //                     "QPlainTextEdit {background-color: black; color: red;}"

     ui->plainTextEdit_edytor->setStyleSheet(s.c_str());

    on_plainTextEdit_edytor_selectionChanged();
    pocz = pocz_;
    kon = kon_;
    value = value_;
//     przewijak->setRange(pocz, kon);
    przewijak->setValue(value);
//    przewijak->setMinimum(pocz);
//    przewijak->setMaximum(kon);
//    ui->plainTextEdit_edytor->setPlainText(txt.c_str());

   // cout << "linijka " << __LINE__ << endl;
    //value_ = 2000;
//    ui->plainTextEdit_edytor->textCursor().setPosition(value);


//    int gdzie =  ui->plainTextEdit_edytor->textCursor().position();

//    string tekst = give_edited().toStdString();
//        cout  << " nabliższy tekst to "
//              << tekst.substr(gdzie, 15)
//         << endl;

    cout << "Na zakończenie funkcji  "<< __PRETTY_FUNCTION__ << " dane przewijaka są : "
           << ", min = " << przewijak->minimum()
           << ", max = " << przewijak->maximum()
           << ", value  = " << przewijak->value()
          << endl;

    tim->start(10);
}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_timer()
{
    QScrollBar *  przewijak = ui->plainTextEdit_edytor->verticalScrollBar();
//    //przewijak->setValue(value);
//    cout
//            <<  " min = "           << przewijak->minimum()
//             <<  " max = "           << przewijak->maximum()
//              <<  " value = "           << przewijak-> value()
//               << endl;

   // przewijak->setRange(pocz, kon);

    int skval = value*  przewijak->maximum() / kon;
    przewijak->setValue(skval);



//    cout
//            <<  __PRETTY_FUNCTION__ << " - Po przesunieciu min = "           << przewijak->minimum()
//             <<  " max = "           << przewijak->maximum()
//              <<  " value = "           << przewijak-> value()
//               << endl;

    ui->plainTextEdit_edytor->textCursor().setPosition(skval);
    tim->stop();
//    cout << " koniec " << __PRETTY_FUNCTION__ << endl;

   cout << " koniec " << __PRETTY_FUNCTION__ << endl;
}
//***********************************************************************************************
void DialogTedycja_bon_motu::daj_pozycje_skrollera(
        int *valptr, int *konptr )
{
    cout << __PRETTY_FUNCTION__ << endl;
     przewijak = ui->plainTextEdit_edytor->verticalScrollBar();
    *valptr = przewijak->value();
    *konptr = przewijak->maximum();

     przewijak->pos();

         cout
                 <<  " min = "           << przewijak->minimum()
                  <<  " max = "           << przewijak->maximum()
                   <<  " value = "           << przewijak-> value()
//                     << ", pos = "
//                     <<   przewijak->pos()


                    << endl;
//         int m = przewijak->

         on_plainTextEdit_edytor_selectionChanged();
         cout << "Pozycja kursora = "
              << ui->plainTextEdit_edytor->textCursor().position()
        << "  Page step = " << przewijak->pageStep()
              << endl;

}

//***********************************************************************************************
// Funkcja, która na zakończenie pracy tego edytującego okna dialogowego
// zdejmuje wyedytowany tekst i zamienia go na tekst z symbolami htmp
// czyli <br> i <b>
//***********************************************************************************************
QString DialogTedycja_bon_motu::give_edited()
{
//    return  ui->plainTextEdit_edytor->toPlainText();
    // zamien karetki na <br>
    std::string txt = ui->plainTextEdit_edytor->toPlainText().toStdString();

   // cout << "odebrany tekst=" << txt << endl;
    string::size_type where = string::npos;

    while(1) {
        where = txt.rfind('\n', where);
        if(where == string::npos) break;
        txt.replace (where, 1, "<br>\n");
    }
     return  txt.c_str();
}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_save_edited_clicked()
{

}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_buttonBox_accepted()
{
cout << "wcisniete cos co ma dac accepted" << endl;
}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_buttonBox_rejected()
{
    cout << "wcisniete cos co ma dac rejected" << endl;
    rejected();

}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_Bold_code_clicked()
{
    // odnalezienie kursora
    auto sel = ui->plainTextEdit_edytor->textCursor();
   int pocz = sel.selectionStart();
   int kon = sel.selectionEnd();

   if(pocz == kon){
    sel.insertText("<b>");
   }

    // wstawienie tem <b>
}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_bold_finish_clicked()
{
    // odnalezienie kursora
    // wstawienie tem </b>
    auto sel = ui->plainTextEdit_edytor->textCursor();
    int pocz = sel.selectionStart();
    int kon = sel.selectionEnd();
    if(pocz == kon){
     sel.insertText("</b>");
    }
}
//***********************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_bold_clicked()
{
    // odnalezienie kursora
    auto sel = ui->plainTextEdit_edytor->textCursor();
   int pocz = sel.selectionStart();
   int kon = sel.selectionEnd();

   if(pocz == kon){
    sel.insertText("<b>");
   }
   else
   {
       auto txt = sel.selectedText();
       sel.insertText(QString("<b>" +txt + "</b>") );
   }
}
//*************************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_not_bold_clicked()
{
    // odnalezienie kursora
    // wstawienie tem </b>
    auto sel = ui->plainTextEdit_edytor->textCursor();
    int pocz = sel.selectionStart();
    int kon = sel.selectionEnd();
    if(pocz == kon){
     sel.insertText("</b>");
    }
    else
    {
        auto txt = sel.selectedText();
        sel.insertText(QString("</b>" +txt + "<b>") );
    }
}
//*************************************************************************************************
void DialogTedycja_bon_motu::on_plainTextEdit_edytor_selectionChanged()
{
   auto sel = ui->plainTextEdit_edytor->textCursor();
   int pocz = sel.selectionStart();
   int kon = sel.selectionEnd();

  ui->pushButton_Bold_code->setEnabled((pocz == kon) );
  ui->pushButton_bold_finish->setEnabled((pocz == kon) );

  ui->pushButton_bold->setEnabled((pocz != kon) );
  ui->pushButton_not_bold->setEnabled((pocz != kon) );

  ui->pushButton_cudzyslow->setEnabled((pocz != kon) );
  ui->pushButton_bez_cudzyslowy->setEnabled((pocz != kon) );

  ui->pushButton_didaskalia->setEnabled((pocz != kon) );

}
//*************************************************************************************************

void DialogTedycja_bon_motu::on_pushButton_cudzyslow_clicked()
{
    // odnalezienie kursora
    auto sel = ui->plainTextEdit_edytor->textCursor();
   int pocz = sel.selectionStart();
   int kon = sel.selectionEnd();

   if(pocz == kon){
    sel.insertText("\"");
   }
   else
   {
       auto txt = sel.selectedText();
       sel.insertText(QString("„" +txt + "”") );
   }

}
//**********************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_bez_cudzyslowy_clicked()
{
    // odnalezienie kursora
    // wstawienie tem </b>
    auto sel = ui->plainTextEdit_edytor->textCursor();
    int pocz = sel.selectionStart();
    int kon = sel.selectionEnd();
    if(pocz == kon){
     sel.insertText("</b>");
    }
    else
    {
        auto txt = sel.selectedText();
        txt.remove("\"" );
        txt.remove("„");
        txt.remove("”");
        sel.insertText(QString(txt) );
    }
}
//************************************************************************************************************
void DialogTedycja_bon_motu::on_pushButton_didaskalia_clicked()
{
    // odnalezienie kursora
    auto sel = ui->plainTextEdit_edytor->textCursor();
   int pocz = sel.selectionStart();
   int kon = sel.selectionEnd();

   if(pocz == kon){
    sel.insertText("\"");
   }
   else
   {
       auto txt = sel.selectedText();
       sel.insertText(QString("[--" +txt + "--]") );
   }
}
