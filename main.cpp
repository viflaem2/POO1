#include <iostream>
#include <string.h>
/**
Proiectul modeleaza gestionarea vanzarilor intr-un magazin de tip teleshop.
Aplicatia permite administrarea clientilor, angajatilor, produselor si facturilor, respectand principiul incapsularii datelor
prin utilizarea atributelor private si a functiilor membre publice pentru acces si modificare controlata.
Sunt implementate functionalitati uzuale de creare, vizualizare si actualizare a obiectelor,
precum si operatii specifice domeniului: emiterea unei facturi,plata unei comenzi si rambursarea partiala sau totala a produselor comandate.
Programul este gandit din perspectiva unui manager.
Am utilizat 4 clase: Produs, Client, Factura, Angajat
Voi explica la final motivul pentru care Factura este clasa prieten cu toate clasele.
Clasa Produs reprezinta Produsele vandute de teleshop, acestea pot fi complet gestionate de Angajati.
Clasa Client reprezinta Clientii teleshopului care pot comanda Produse si pot plati sau cere rambursarea unei Facturi
Clasa Angajat reprezinta Angajatii teleshopului. Functia lor principala e sa preia comenzile Clientilor si sa emita o Factura.
Clasa Factura reprezinta Factura emisa in urma unei vanzari. Acesta este clasa prieten cu toate clasele deoarece acesta modifica statisticile
/stocul Produselor care in mod normal nu sunt direct accesibile.
Logica generala este dupa cum urmeaza: Un Client poate comanda X produse de la un Angajat. Anagajtul emite Factura si o trimite Clientului.
Clientul poate Plati factura. Daca clientul se razgandeste, Clientul poate cere o rambursare partiala sau complete a unei Facturi.
 **/
class Factura;
class Angajat;

class Produs{
friend Angajat;
friend Factura;
private:
    static int contorProdusId;
    unsigned long long stoc;
    const int produsId;
    char *numeProdus;
    float pret,discount;
    double greutate;

    void SetStoc(long long int &add){
        if(add<=0){
            printf("Valoare de stoc invalida\n");
            return;
        }
        this->stoc+=add;
    }

    void SetDiscount(float &discount){
        if(discount<0||discount>100){
            printf("Valoare de discount invalida\n");
            return;
        }
        this->discount=discount;
    }

    void SetDetails(const char *nume, float pret, double greutate){
        delete[] numeProdus;
        numeProdus=new char[strlen(nume)+1];
        strcpy(numeProdus,nume);
        this->pret=pret;
        this->greutate=greutate;
    }

public:
    Produs(const char *nume, float pret, double greutate, unsigned long long stoc=0, float discount=0):produsId(contorProdusId++){
        numeProdus=new char[strlen(nume)+1];
        strcpy(numeProdus,nume);
        this->pret=pret;
        this->greutate=greutate;
        this->stoc=stoc;
        this->discount=discount;
    }

    Produs(const Produs &decopiat):produsId(contorProdusId++){
        this->numeProdus=new char[strlen(decopiat.numeProdus)+1];
        strcpy(this->numeProdus,decopiat.numeProdus);
        this->pret=decopiat.pret;
        this->greutate=decopiat.greutate;
        this->stoc=decopiat.stoc;
        this->discount=decopiat.discount;
    }

    ~Produs(){
        delete[] numeProdus;
    }
    Produs& operator=(const Produs& deCopiat) {
        if(this!=&deCopiat){
            delete[] this->numeProdus;
            this->numeProdus=new char[strlen(deCopiat.numeProdus)+1];
            strcpy(this->numeProdus,deCopiat.numeProdus);
            this->pret=deCopiat.pret;
            this->discount=deCopiat.discount;
            this->greutate=deCopiat.greutate;
            this->stoc=deCopiat.stoc;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Produs &p){
        out<<"Produs id: "<<p.produsId<<" , nume: "<<p.numeProdus<<" , pret: "<<p.pret<<" , discount: "<<p.discount<<" , greutate: "<<p.greutate<< " , stoc: " << p.stoc;
        return out;
    }

    void afisareProdus() const{
        float pretFinal=pret*(1-discount/100);
        if (discount>0)
            printf("%s - %.2f lei (Stoc: %llu), redus de la %.2f lei\n",numeProdus,pretFinal,stoc,pret);
        else
            printf("%s - %.2f lei (Stoc: %llu)\n",numeProdus,pretFinal,stoc);
    }

};

class Client{
friend Factura;
friend Angajat;
private:
    static int contorClientId;
    char *numeComplet, *adresaLivrare;
    char numarTelefon[20];
    const int clientId;
    float totalCheltuit;
    int nrComenziEfectuate;

    void updateStatistici(float sumaComanda) {
        if (sumaComanda>0){
            this->totalCheltuit+=sumaComanda;
            this->nrComenziEfectuate++;
        }
    }

    void vizualizareStatisticiClient() const {
        printf("Client %d, Total cheltuit: %.2f lei, Numar comenzi finalizate: %d\n", clientId,totalCheltuit,nrComenziEfectuate);
    }

public:
    Client(const char *nume, const char *adresa, const char *numar):clientId(contorClientId++){
        this->numeComplet = new char[strlen(nume)+1];
        strcpy(this->numeComplet,nume);
        this->adresaLivrare = new char[strlen(adresa)+1];
        strcpy(this->adresaLivrare,adresa);
        strncpy(this->numarTelefon,numar,19); //ca sa previn buffer overflow
        this->numarTelefon[19]='\0';
        this->totalCheltuit=0;
        this->nrComenziEfectuate=0;
        }

    ~Client() {
        delete[] numeComplet;
        delete[] adresaLivrare;
    }

    Client(const Client &decopiat):clientId(contorClientId++){
        this->numeComplet=new char[strlen(decopiat.numeComplet)+1];
        strcpy(this->numeComplet,decopiat.numeComplet);
        this->adresaLivrare=new char[strlen(decopiat.adresaLivrare)+1];
        strcpy(this->adresaLivrare,decopiat.adresaLivrare);
        strncpy(this->numarTelefon,decopiat.numarTelefon,19);
        this->numarTelefon[19]='\0';
        this->totalCheltuit=decopiat.totalCheltuit;
        this->nrComenziEfectuate=decopiat.nrComenziEfectuate;
    }
    Client& operator=(const Client &deCopiat) {
        if(this!=&deCopiat){
            delete[] this->numeComplet;
            delete[] this->adresaLivrare;
            this->numeComplet=new char[strlen(deCopiat.numeComplet)+1];
            strcpy(this->numeComplet,deCopiat.numeComplet);
            this->adresaLivrare=new char[strlen(deCopiat.adresaLivrare)+1];
            strcpy(this->adresaLivrare,deCopiat.adresaLivrare);
            strncpy(this->numarTelefon,deCopiat.numarTelefon,19);
            this->numarTelefon[19]='\0';
            this->totalCheltuit=deCopiat.totalCheltuit;
            this->nrComenziEfectuate=deCopiat.nrComenziEfectuate;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Client &c){
        out<<"Client id: "<<c.clientId<<" , nume complet: "<<c.numeComplet<<" , adresa livrare: "<<c.adresaLivrare<<" , numar telefon: "<<c.numarTelefon<<" , total cheltuit: "<<c.totalCheltuit<<" , nr comenzi efectuate: "<<c.nrComenziEfectuate;
        return out;
    }

    void vizualizareDateClient() const {
        printf("Client: %s | ID: %d | Tel: %s | Adresa: %s\n", numeComplet, clientId, numarTelefon, adresaLivrare);
    }

    void schimbareAdresaLivrare(const char* adresaNoua){
        delete[] adresaLivrare;
        adresaLivrare=new char[strlen(adresaNoua)+1];
        strcpy(adresaLivrare,adresaNoua);
    }

    void schimbareNumarTelefon(const char* telefonNou){
         strncpy(numarTelefon,telefonNou,19);
        numarTelefon[19]='\0';
    }

    void afiseazaStareFactura(const Factura& f) const;
};

class Angajat {
friend Factura;
private:
    static int contorAngajatId;
    const int angajatId;
    char* nume;
    unsigned int numarComenziPreluate;
public:
    Angajat(const char* numeAngajat):angajatId(contorAngajatId++){
        this->nume=new char[strlen(numeAngajat)+1];
        this->numarComenziPreluate=0;
        strcpy(this->nume, numeAngajat);
    }

    ~Angajat() {
        delete[] nume;
    }

    Angajat(const Angajat& deCopiat):angajatId(contorAngajatId++) {
        this->nume=new char[strlen(deCopiat.nume)+1];
        strcpy(this->nume,deCopiat.nume);
        this->numarComenziPreluate=deCopiat.numarComenziPreluate;
    }
    Angajat& operator=(const Angajat& deCopiat) {
        if(this!=&deCopiat){
            delete[] this->nume;
            this->nume=new char[strlen(deCopiat.nume)+1];
            strcpy(this->nume,deCopiat.nume);
            this->numarComenziPreluate=deCopiat.numarComenziPreluate;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Angajat &a){
        out<<"Angajat id: "<<a.angajatId<<" , nume: "<<a.nume<<" , numar comenzi preluate: "<<a.numarComenziPreluate;
        return out;
    }
    void afisareStatistici() const {
        printf("ID Angajat: %d, Nume: %s , Comenzi finalizate: %u\n", angajatId, nume, numarComenziPreluate);
    }

    void vizualizareStatisticiClient(const Client& c) const {
        c.vizualizareStatisticiClient();
    }

    void setStocProdus(Produs& p, long long int add) {
        p.SetStoc(add);
    }

    void setDetailsProdus(Produs& p,const char *nume, float pret, double greutate){
        p.SetDetails(nume,pret,greutate);
    }

    void setDiscountProdus(Produs& p, float discount) {
        p.SetDiscount(discount);
    }

    Factura emiteFactura(Client& c, Produs& p, unsigned int bucati);


};

class Factura {
private:
    static int contorFacturaId;
    const int facturaId;
    Client* client;
    Angajat* angajat;
    Produs* produs;
    unsigned int cantitate;
    bool estePlatita;

    friend class Angajat;

public:
    Factura(Client& c, Angajat& a, Produs& p, unsigned int bucati):facturaId(contorFacturaId++),client(&c),angajat(&a),produs(&p){
        this->cantitate=bucati;
        this->estePlatita=false;
    }
    Factura(const Factura &deCopiat):facturaId(contorFacturaId++),client(deCopiat.client),angajat(deCopiat.angajat),produs(deCopiat.produs){
        this->cantitate=deCopiat.cantitate;
        this->estePlatita=deCopiat.estePlatita;
    }
    Factura& operator=(const Factura& deCopiat) {
        if(this!=&deCopiat){
            this->client=deCopiat.client;
            this->angajat=deCopiat.angajat;
            this->produs=deCopiat.produs;
            this->cantitate=deCopiat.cantitate;
            this->estePlatita=deCopiat.estePlatita;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Factura &f){
        out<<"Factura id: "<<f.facturaId<<" , client id: "<<f.client->clientId<<" , angajat id: "<<f.angajat->angajatId<<" , produs id: "<<f.produs->produsId<<" , cantitate: "<<f.cantitate<<" , status plata: "<<(f.estePlatita ? "da" : "nu");
        return out;
    }

    float calculeazaTotal() const {
        float pretFinalUnitate=produs->pret*(1-produs->discount/100);
        return pretFinalUnitate*cantitate;
    }

    void plata() {
        if(estePlatita)
            return;
        if(produs->stoc>=cantitate) {
            float total=calculeazaTotal();
            produs->stoc-=cantitate;
            client->updateStatistici(total);
            angajat->numarComenziPreluate++;
            this->estePlatita=true;
        }
    }

    void rambursare(int bucati){
        if(!estePlatita)
                    return;
        if(bucati>cantitate)
                    return;
        if(bucati<=0)
            return;
        float sumaRambursata=produs->pret*(1-produs->discount/100);
        produs->stoc+=bucati;
        client->totalCheltuit-=sumaRambursata;
        cantitate-=bucati;
        if(cantitate==0){
            client->nrComenziEfectuate--;
            angajat->numarComenziPreluate--;
        }
    }

    int getId() const {return facturaId;}
    bool statusPlata() const {return estePlatita;}
};

int Factura::contorFacturaId=0;
int Angajat::contorAngajatId=0;
int Produs::contorProdusId=0;
int Client::contorClientId=0;

Factura Angajat::emiteFactura(Client& c, Produs& p, unsigned int bucati){
    Factura facturaNoua = Factura(c, *this, p, bucati);
    printf("Se emite factura nr. %d, Total de plata: %.2f lei\n", facturaNoua.getId(),facturaNoua.calculeazaTotal());
    return facturaNoua;
}

void Client::afiseazaStareFactura(const Factura& f) const{
        if(f.statusPlata()){
            printf("Factura este platita.\n");
        }
        else{
            printf("Factura NU este platita.\n");
        }
}
int main() {
    Angajat managerStoc("Mihai Popescu");
    Angajat operatorVanzari("Elena Ionescu");

    Produs laptop("Laptop Gaming",8200.0,2.4,8);
    Produs casti("Casti Wireless",450.0,0.25,25);
    Produs ceas("Ceas Smart",1300.0,0.18,12);

    Client client1("Andrei Georgescu","Str. Lalelelor 10, Bucuresti","0711111111");
    Client client2("Maria Dumitrescu","Str. Libertatii 25, Cluj","0722222222");

    laptop.afisareProdus();
    casti.afisareProdus();
    ceas.afisareProdus();

    managerStoc.setStocProdus(laptop, 5);
    managerStoc.setDiscountProdus(casti, 10);
    managerStoc.setDetailsProdus(ceas, "Ceas Smart Pro", 1499.0f, 0.20);

    laptop.afisareProdus();
    casti.afisareProdus();
    ceas.afisareProdus();

    client1.schimbareAdresaLivrare("Bd. Unirii 100, Bucuresti");
    client1.schimbareNumarTelefon("0799999999");
    client1.vizualizareDateClient();

    Factura facturaLaptop = operatorVanzari.emiteFactura(client1, laptop, 1);

    printf("Factura emisa are ID-ul: %d\n", facturaLaptop.getId());
    printf("Total de plata: %.2f lei\n", facturaLaptop.calculeazaTotal());
    client1.afiseazaStareFactura(facturaLaptop);

    facturaLaptop.plata();
    client1.afiseazaStareFactura(facturaLaptop);
    laptop.afisareProdus();

    Factura facturaCasti = operatorVanzari.emiteFactura(client2, casti, 2);
    printf("Factura casti - total: %.2f lei\n", facturaCasti.calculeazaTotal());
    facturaCasti.plata();
    casti.afisareProdus();
    facturaCasti.rambursare(1);

    casti.afisareProdus();
    client2.afiseazaStareFactura(facturaCasti);

    Factura facturaCeas = operatorVanzari.emiteFactura(client2, ceas, 1);
    facturaCeas.plata();
    ceas.afisareProdus();

    managerStoc.afisareStatistici();
    operatorVanzari.afisareStatistici();

    operatorVanzari.vizualizareStatisticiClient(client1);
    operatorVanzari.vizualizareStatisticiClient(client2);
    std::cout<<laptop<<"\n";
    std::cout<<casti<<"\n";
    std::cout<<ceas<<"\n";
    std::cout<<client1<<"\n";
    std::cout<<client2<<"\n";
    std::cout<<operatorVanzari<<"\n";
    std::cout<<facturaLaptop<<"\n";
    std::cout<<facturaCasti<<"\n";
    std::cout<<facturaCeas<<"\n";

    return 0;
}
