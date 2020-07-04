#ifndef BMP280T_H
#define BMP280T_H


#if defined(ARDUNIO) && (ARDUNIO>=100)
#include <Ardunio.h>
#endif

/* 
  --------- DİKKAT ONCE BURAYI OKU-----------
  
    ilk yükseklik değeri hesaplanamaz öncelikle basinçla sıcaklığı bulman gerekiyor
    bunun sebebini şöyle açıklayalım

    önce basınç ve sıcaklığı bulduğunu varsayalım
    bu değerleri kullanarak da yüksekliği buluyoruz
    ne güzel di mi gerektiği gibi çalışıyor O AN Kİ yüksekliği böylece bulmuş oluyoruz


    ama eğer yüksekliğin içinde bu fonksiyonları çağırmış olsaydım
    sıcaklığı bulacak basıncı bulacak ve sonraki adımın yüksekliğini bulacaktı bu da istediğimiz bir şey değil tabi kide
    yani o anki yüksekşiği değil sonraki değerlerin yüksekliğini bulmuş olacaktı
    saçma dimi
    katılıyorum

    her neyse devam edelim.

    



*/



class Bmp280T{

  public:


    //bu i2c için gerekli bağlantıları yapan fonksiyon ilk bunun çağrılması lazım 
    //ama dikkat et bunu setup in içine yazacaksın ayrıca
    //bunu Wire.begin()' nin de altına yazman gerekiyor bunun sebebi fonksiyonun içinde Wire i kullanılmış olması
    void baglan();//ayrıca bu fonksiyonun içinde getBasinc() fonksiyonunu çağrıyorum bunun sebebi İLK BASINCI bulup onu
    //değişkine eşitlemek neden bunu yapıyorum diye soracak olursan da bunun sebebi yukseklik değerini hesaplarken ilk basınç değerini kullanıyor olmamız
    


    //basıncı hesaplayıp double olarak döndürüyor
    double getBasinc();

    //sicakliği hesaplayıp double olarak döndürüyor
    double getSicaklik();

  
    //burda yüksekliği bulup döndürüyor ama hesaplarken basinc,sicakliği ve ilk basinc değerini kullanmasi gerekiyor
    double getYukseklik();//bunun için altta gördüğün 3 değişkenim var

    //baglan() fonksiyonu çalışarak sensör ile gereken bağlantı kuruluyor ve ilk basinc değeri bulunup
    double ilkBasincDegeri=0;//bu değişkene atanıyor
    
    double sicaklik=0;//getSicaklik() fonksiyonu çalıştırarak çıkan sonuc döndürülüyor ayrıca o değer bu değişkene atanıyor
    //bunun sebebini yukarda anlattım O AN Kİ sicakliği bulmuş olduk bundan sonraki işlemde getYukseklik()'i çağırdımızda bu değişkenş kullanıyor böylece
    //O AN Kİ Yüksekliği bulmuş oluyoruz

    
    double basinc=0;//getBasinc() fonksiyonu çalıştırarak çıkan sonuc döndürülüyor ayrıca o değer bu değişkene atanıyor
    //bunun sebebini yukarda anlattım O AN Kİ basinci bulmuş olduk bundan sonraki işlemde getYukseklik()'i çağırdımızda bu değişkenş kullanıyor böylece
    //O AN Kİ Yüksekliği bulmuş oluyoruz

  
  
};






#endif
