//====================================================================
// Simulation dynamischer Systeme mit PLAN
//====================================================================
// Projektbeschreibung:
// Implementierung der LKW-Kolonnnenfahrt auf Autobahn
// Daten/Ort:
// 06.12.2018, Braunschweig
// ===================================================================
// Projektteilnehmer:
// Name		      Matrikel-Nr.
// Shi, Ran		4825188
// Gao, Pengxiao	4872458
// Wang, Kai		4934820

//====================================================================
// Bibliotheken
#include <vcl.h>
#pragma hdrstop
#include "Plan.h"
#include <math.h>

//====================================================================
//Programmanfang
//====================================================================

// 1. Definieren Parameter
//====================================================================
//= Beschreibung:
//= Parameter von Fahrzeuge und Fahrbahn difinieren
//====================================================================

//==== Fahrspur
float LinieBereich,SpurBereich,NotSpurBereich,LinieLaenge,LinieKlar;
//==== Fahrzeug
float LkwLaenge,LkwBereich,VerkehrLaenge,VerkehrBereich;
//==== Abbiegen
int Nlenkung = 90;
int Count0[3] = {1,1,1};
int Count1[3]= {0,0,0};
int Step = 25;
int t = 0;

// 2. Definieren Struct TUser
//====================================================================
//= Beschreibung:
//= Structure
//====================================================================
class TUser : public TPlan {
        float xKoord,yKoord,xSpur[3];
        float xLkw [3], yLkw [3], Abstand[2], vMax, vIdeal, SicherheitsAbstand;
        int Richtung[3], nSpur[3], Brems[3], yLenkrad[3], RadarOn, Tag, AutoFahren, Bau[3], Polizei;
        float Geschwindigkeit [3];
        float Verkehr[4],yVerkehr[6],vVerkehr[3],vIst;
        int Menu;
public:
        void Init(void);
        void Init_BasicDaten(void);
        void Run(void);
        void Reset(void);
        void Hauptmenu(void);
        void Manuellsmenu(void);
        void AutoFahrensmenu(void);
        void Speziellsmenu(void);
        void Parametersmenu(void);
        //=====
        void RunTaste0(void);
        void RunTaste1(void);
        void RunTaste2(void);
        void RunTaste4(void);
        void RunTaste5(void);
        void RunTaste6(void);
        void RunTaste13(void);
        //==== Fahrspur
        void Draw_Spur(void);
        //==== Fahrzeuge
        void Draw_Auto(void);
        //==== Position Fahrzeuge
        void Berechnen_Abmessung(void);
        //==== Ablenkung
        void Ablenkung(int count,int num_Auto);
        //==== Richtungflag
        void Berechnen_Richtung(void);
        //==== Zufaellige Fahrzeuge
        void Draw_Verkehr(void);
        //==== Automatisierte Kolonnenfahrt
        void Auto_Fahren(void);
        //==== Fahrsituation
        void IsTag(void);
        //==== Fernlicht
        void Draw_Scheinwerfer(int x,int y,int B,int L);
        //==== Fahrrichtungsanzeiger
        void Draw_Blinker(int i,int k);
        //==== Bremsleuchte
        void Draw_Bremslamp(int i);
        //==== Tachometer
        void Draw_Tempomesser(void);
        //==== Lenkrad
        void Draw_Lenkrad(void);
        //==== Spurassistent
        void Draw_Spurassistent(void);
        //==== Radar
        void Draw_Radar(void);
        //==== HUD
        void Draw_HUD(void);
        //==== Sensorerfassung
        void Sensordaten(void);
        void Draw_Tagwechsel(void);
        void Draw_Pedal(void);
        void BildMouseDown(int x, int y);
        void BildMouseMove(int x, int y, int leftdown);
        //==== Plot Geschwindigkeit
        void V_Anzeige(void);
        //==== Plot Kraftstoffverbrauch
        void KSV_Anzeige(void);
        void Plot_Diagramm(void);
        void Dynamik_Plot(void);
        void Draw_Polizei(void);
        void Draw_Bauwerk(void);
};

// 3. Definieren Public Funktion von TUser
//========================================================================
//= Funktionsname:
//=                Init_BasicDaten()
//= Beschreibung:
//= Parameter eingeben
//========================================================================
void TUser::Init_BasicDaten(void)
{
        xKoord = 60.0;
        yKoord = 10.0;
        //==== Fahrspur
        LinieBereich = 4.0;
        SpurBereich = 18.75*LinieBereich;
        NotSpurBereich = 16.0*LinieBereich;
        LinieLaenge = 30.0*LinieBereich;
        LinieKlar = 75.0*LinieBereich;
        LkwLaenge = 40.0*LinieBereich;
        LkwBereich = 11.5*LinieBereich;
        //==== Position
        SicherheitsAbstand = 1.3;
        xSpur[0]=xKoord+LinieBereich*4.0;
        xSpur[1]=xKoord+SpurBereich+LinieBereich*5.0;
        xSpur[2]=xKoord+SpurBereich*2.0+LinieBereich*6.0;
        xLkw[0] = xLkw[1] = xLkw[2] = xSpur[2];
        nSpur[0]=nSpur[1]=nSpur[2]=2;
        yLkw[0] = 300;
        yLkw[1] = 300+SicherheitsAbstand*LkwLaenge;
        yLkw[2] = yLkw[1]+SicherheitsAbstand*LkwLaenge;
        //==== Geschwindigkeit
        vMax = 15;   //135km/h
        vIdeal = 11; //99km/h
        Geschwindigkeit[0] = Geschwindigkeit[1] = Geschwindigkeit[2] = vIdeal;
        //==== Abstand
        Abstand[0]=Abstand[1]=0;
        //==== Richtungsflag
        Richtung[0]=Richtung[1]=Richtung[2]= 0;
        //==== Verkehr
        Verkehr[0]=0;
        vVerkehr[0]=vVerkehr[1]=vVerkehr[2]=0;
        Verkehr[1]=Verkehr[2]=Verkehr[3]=0;
        VerkehrLaenge =20*LinieBereich;
        VerkehrBereich=10*LinieBereich;
        yVerkehr[0]=yVerkehr[1]=yVerkehr[2]=yVerkehr[3]=yVerkehr[4]=yVerkehr[5]=-100;
        AutoFahren = -1;
        //==== Lenkrad
        yLenkrad[0]=270;
        yLenkrad[1]=570;
        yLenkrad[2]=870;
        RadarOn=-1;
        Tag=1;
        Bau[0]=Bau[1]=Bau[2]=-1;
        Polizei=0;
}

//========================================================================
//= Funktionsname:
//=                Draw_Spur()
//= Beschreibung:
//= Fahrspur zeichnen
//========================================================================
void TUser::Draw_Spur(void)
{
        //==== Fensterbegrenzung difinieren
        View(int(xKoord-LinieBereich),10,int(5.0*LinieBereich+3.0*SpurBereich+NotSpurBereich),1000);
        Clear(Schwarz);
        //==== Fahrspurbegrenzung definieren
        IsTag();
        SetPen(Weiss);
        SetBrush(Weiss);
        //==== Notfallspur
        Rectangle(int(xKoord+3.0*SpurBereich+2.0*LinieBereich),10,int(LinieBereich),1000);
        //==== Alle Fahrspurmarkierungungen f��r bestimmte Zeit zeichnen, Zylkusnummer 1000/(LinieBereich*LinieKlar)
        for(int i = 0;i <6;i++)
        {
                //==== Wenn Startkoordination groesser als Markierungslaenger, zrueckstellen
                if(yKoord >= (LinieKlar+10.0))
                        yKoord = 10.0;
                //====Zwei Fahrspurmarkierungen zeichnen
                Rectangle(int(xKoord+SpurBereich),int(yKoord+LinieKlar*i),int(LinieBereich),-int(LinieLaenge));
                Rectangle(int(xKoord+SpurBereich*2.0+LinieBereich),int(yKoord+LinieKlar*i),int(LinieBereich),-int(LinieLaenge));

        }
        yKoord += Geschwindigkeit[0];
        Draw_Bauwerk();
}

//========================================================================
//= Funktionsname:
//=                Draw_Bauwerk()
//= Beschreibung:
//= Bauwerk
//========================================================================
void TUser::Draw_Bauwerk(void)
{
//====
        for(int i=0;i<3;i++)
        {
                if(Bau[i]==1)
                {
                        Verkehr[i+1]=1;
                        yVerkehr[i]=-100;
                        if(Bau[i]==1&&nSpur[0]==i&&(yLkw[0]>yVerkehr[nSpur[0]-1]+VerkehrLaenge))
                        {
                                AutoFahren=1;
                                Richtung[0]=-1;
                        }
                        else if(nSpur[0]==1)
                                AutoFahren=-1;


                        SetBrushColor(Hellrot);
                        for(int k=0;k<3;k++)
                        {
                                ClearPoints();
                                SetPoint(int(xSpur[i]),200+300*k);
                                SetPoint(int(xSpur[i])+3,197+300*k);
                                SetPoint(int(xSpur[i])+50,247+300*k);
                                SetPoint(int(xSpur[i])+47,250+300*k);
                                Poly();
                                ClearPoints();
                                SetPoint(int(xSpur[i]),247+300*k);
                                SetPoint(int(xSpur[i])+3,250+300*k);
                                SetPoint(int(xSpur[i])+50,200+300*k);
                                SetPoint(int(xSpur[i])+47,197+300*k);
                                Poly();
                        }
                }
        }
}
//========================================================================
//= Funktionsname:
//=                Draw_Auto()
//= Beschreibung:
//= Fahrzeug zeichnen, Sensorerfassungsbereich, Fahrrichtungsanzeiger und Bremsleuchte
//========================================================================
void TUser::Draw_Auto()
{
        SetPen(Schwarz);
        //==== LKW
        SetBrush(Rot);
        Rectangle(int(xLkw[0]),int(yLkw[0]),int(LkwBereich),int(LkwLaenge));
        SetBrush(Gruen);
        Rectangle(int(xLkw[1]),int(yLkw[1]),int(LkwBereich),int(LkwLaenge));
        SetBrush(Blau);
        Rectangle(int(xLkw[2]),int(yLkw[2]),int(LkwBereich),int(LkwLaenge));

        for(int i=0;i<3;i++)
        {
                if(Richtung[i]==-1&&Count0[i]%10<=5)
                        Draw_Blinker(i,-1);
                else if(Richtung[i]==1&&Count0[i]%10<=5)
                        Draw_Blinker(i,1);
        }
        //==== Bremsleuchte
        for(int i=0;i<3;i++)
        {
                if(Brems[i]==1)
                        Draw_Bremslamp(i);
        }
        if(RadarOn==1) Draw_Radar();
        if(Polizei==1) Draw_Polizei();
}

void TUser::IsTag(void)
{
        if(Tag==-1)
        {
                View(int(xKoord),10,int(3.0*LinieBereich+3.0*SpurBereich+NotSpurBereich),1000);
                Clear(Schwarz);

                //SicherheitsAbstand = 1.5;
                for(int i=0;i<3;i++)
                {
                        Draw_Scheinwerfer(int(xLkw[i]),int(yLkw[i]),int(LkwBereich),int(LkwLaenge));
                        Draw_Scheinwerfer(int(xSpur[i]),int(yVerkehr[i]),int(VerkehrBereich),int(VerkehrLaenge));
                        Draw_Scheinwerfer(int(xSpur[i]),int(yVerkehr[i+3]),int(VerkehrBereich),int(VerkehrLaenge));
                }
        }
        else
        {
                View(int(xKoord),10,int(3.0*LinieBereich+3.0*SpurBereich+NotSpurBereich),1000);
                Clear(Grau);
                SicherheitsAbstand = 1.3;
        }
}

void TUser::Draw_Scheinwerfer(int x,int y,int B,int L)
{

        SetPen(Gelb);
        SetBrush(Gelb);
        ClearPoints();
        SetPoint(x,y);
        SetPoint(x+75,y-130);
        SetPoint(x-75,y-130);
        Poly();

        ClearPoints();
        SetPoint(x+B,y);
        SetPoint(x+B+75,y-130);
        SetPoint(x+B-75,y-130);
        Poly();
        SetPenSize(5);
        for(int k=1;k<5;k++)
        {
                Arc(x,y-3,60,120,150-5*k);
                Arc(x+B,y-3,60,120,150-5*k);
        }

        SetPen(Hellrot);
        SetPenSize(1);
        SetBrushColor(Hellrot);
        Rectangle(x,y+L,B,int(1*LinieBereich));
}

void TUser::Draw_Radar(void)
{
        SetPen(Hellgelb);
        SetBrush(Klar);
        //==== Front Erfassungsbereich
        MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]));
        LineTo(int(xLkw[1]+0.5*LkwBereich+75),int(yLkw[1]-130));
        MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]));
        LineTo(int(xLkw[1]+0.5*LkwBereich-75),int(yLkw[1]-130));
        Arc(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]),60,120,150);
        //====links
        MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
        LineTo(int(xLkw[1]-50),int(yLkw[1]+0.5*LkwLaenge-86.6));
        MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
        LineTo(int(xLkw[1]-50),int(yLkw[1]+0.5*LkwLaenge+86.6));
        Arc(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge),120,240,100);
        //==== rechts
        MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
        LineTo(int(xLkw[1]+LkwBereich+50),int(yLkw[1]+0.5*LkwLaenge-86.6));
        MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
        LineTo(int(xLkw[1]+LkwBereich+50),int(yLkw[1]+0.5*LkwLaenge+86.6));
        Arc(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge),300,420,100);
        //==== Hinten
        MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge));
        LineTo(int(xLkw[1]+0.5*LkwBereich+56.6),int(yLkw[1]+LkwLaenge+56.6));
        MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge));
        LineTo(int(xLkw[1]+0.5*LkwBereich-56.6),int(yLkw[1]+LkwLaenge+56.6));
        Arc(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge),225,315,80);
        //==== Punktmessungsfunktion aufrufen
        Sensordaten();

}
//========================================================================
//= Funktionsname:
//=                Draw_Blinker()
//= Beschreibung:
//= Fahrrichtungsanzeiger
//========================================================================
void TUser::Draw_Blinker(int i,int k)
{
        if(k==-1)
        {
                SetPen(Schwarz);
                SetBrush(Gelb);
                Rectangle(int(xLkw[i]),int(yLkw[i]),int(4*LinieBereich),int(4*LinieBereich));
                Rectangle(int(xLkw[i]),int(yLkw[i]+LkwLaenge),int(4*LinieBereich),int(-4*LinieBereich));
        }
        else if(k==1)
        {
                SetPen(Schwarz);
                SetBrush(Gelb);
                Rectangle(int(xLkw[i]+LkwBereich),int(yLkw[i]),int(-4*LinieBereich),int(4*LinieBereich));
                Rectangle(int(xLkw[i]+LkwBereich),int(yLkw[i]+LkwLaenge),int(-4*LinieBereich),int(-4*LinieBereich));
        }
}

//========================================================================
//= Funktionsname:
//=                Draw_Bremslamp()
//= Beschreibung:
//= Bremsleuchte
//========================================================================
void TUser::Draw_Bremslamp(int i)
{
        SetPen(Schwarz);
        SetBrush(Hellrot);
        Rectangle(int(xLkw[i]),int(yLkw[i]+LkwLaenge),int(4*LinieBereich),int(2*LinieBereich));
        Rectangle(int(xLkw[i]+LkwBereich),int(yLkw[i]+LkwLaenge),int(-4*LinieBereich),int(2*LinieBereich));
}

//========================================================================
//= Funktionsname:
//=                Berechnen_Abmessungen()
//= Beschreibung:
//= Abstandmessung
//========================================================================
void TUser::Berechnen_Abmessung(void)
{
        yLkw[1] += Geschwindigkeit[0]-Geschwindigkeit[1];
        yLkw[2] += Geschwindigkeit[0]-Geschwindigkeit[2];
        Abstand[0] = yLkw[1]-yLkw[0]-SicherheitsAbstand*LkwLaenge;
        Abstand[1] = yLkw[2]-yLkw[1]-SicherheitsAbstand*LkwLaenge;
        if(Geschwindigkeit[1]>Geschwindigkeit[0]&&yLkw[1]<(yLkw[0]+1.1*LkwLaenge))
        {
                Brems[1] = 1;
                Geschwindigkeit[1]=Geschwindigkeit[0]+0.005*Abstand[0];

        }
        else if(yLkw[1]>(yLkw[0]+1.5*LkwLaenge)&&Geschwindigkeit[1]<=vMax)
        {
                Brems[1] = 0;
                Geschwindigkeit[1]=Geschwindigkeit[0]+0.005*Abstand[0];
        }
        else if (Abstand[0]>=0&&Abstand[0]<=0.5)
        {
                Brems[1] = 0;
                Geschwindigkeit[1]=Geschwindigkeit[0];
        }

        if(Geschwindigkeit[2]>Geschwindigkeit[1]&&yLkw[2]<(yLkw[1]+1.1*LkwLaenge))
        {
                Brems[2] = 1;
                Geschwindigkeit[2]=Geschwindigkeit[1]+0.005*Abstand[1];

        }
        else if(yLkw[2]>(yLkw[1]+1.5*LkwLaenge)&&Geschwindigkeit[2]<=vMax)
        {
                Brems[2] = 0;
                Geschwindigkeit[2]=Geschwindigkeit[1]+0.005*Abstand[1];
        }
        else if (Abstand[1]>=0&&Abstand[1]<=0.5)
        {
                Brems[2] = 0;
                Geschwindigkeit[2]=Geschwindigkeit[0];

        }
}

//========================================================================
//= Funktionsname:
//=                Ablenkung()
//= Beschreibung:
//= Spurwechsel
//========================================================================
void TUser::Ablenkung(int Num,int num_Auto)
{
        switch(Num)
        {
                case 10: xLkw[num_Auto]+=1*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 20: xLkw[num_Auto]+=2*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 30: xLkw[num_Auto]+=3*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 40: xLkw[num_Auto]+=4*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 50: xLkw[num_Auto]+=5*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 60: xLkw[num_Auto]+=4*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 70: xLkw[num_Auto]+=3*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 80: xLkw[num_Auto]+=2*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
                case 90: xLkw[num_Auto]+=1*Richtung[num_Auto]*(SpurBereich+LinieBereich)/Step;
                        break;
               // break;
        }
}

//========================================================================
//= Funktionsname:
//=                Berechnen_Richtung()
//= Beschreibung:
//= Richtungsflag veraendern
//========================================================================
void TUser::Berechnen_Richtung(void)
{
        for(int i=0;i <3;i++)
        {
                if(Richtung[i]!=0&&Count0[i]<=Nlenkung)
                {
                        Ablenkung(Count0[i],i);
                        Count0[i]++;

                }
                else if(Count0[i] > Nlenkung)
                {
                        nSpur[i]+=Richtung[i];
                        Richtung[i] = 0;
                        Count0[i] = 1;
                }
        }
        if(AutoFahren!=1)
                for(int i=1;i<3;i++)
                {
                        if(xLkw[i]>xLkw[i-1]+0.5*LkwBereich)
                                Richtung[i] = -1;
                        else if(xLkw[i]<xLkw[i-1]-0.5*LkwBereich)
                                Richtung[i]= 1;
                }
        else  if(Verkehr[0]==1)
                for(int i=1;i<3;i++)
                {
                        if(xLkw[i]>xLkw[i-1]+0.5*LkwBereich&&yLkw[i]>yVerkehr[1])
                                Richtung[i] = -1;
                        else if(xLkw[i]<xLkw[i-1]-0.5*LkwBereich&&(yLkw[i]+1.2*LkwLaenge)<yVerkehr[2])
                                Richtung[i]= 1;
                }
        else  if(Verkehr[0]==2)
                for(int i=1;i<3;i++)
                {
                        if(xLkw[i]>xLkw[i-1]+0.5*LkwBereich&&(yLkw[i]>yVerkehr[4]||yVerkehr[1]>800))
                                Richtung[i] = -1;
                        else if(xLkw[i]<xLkw[i-1]-0.5*LkwBereich&&((yLkw[i]+1.2*LkwLaenge)<yVerkehr[5]||yLkw[i]>(2*LkwLaenge+yVerkehr[5])))
                                Richtung[i]= 1;
                }
}

//========================================================================
//= Funktionsname:
//=                Draw_Verkehr()
//= Beschreibung:
//= Zufaeige Verkehrsteinehmer
//========================================================================
void TUser::Draw_Verkehr(void)
{       //====low Verkehr
        if(Verkehr[0]==1)
        {
                if(Verkehr[1]!=1&&Count1[0]%80==0)
                {
                        vVerkehr[0]=vMax-random(3);
                        Verkehr[1]=1;
                        yVerkehr[0]=1000;
                }
                if(Verkehr[2]!=1&&Count1[1]%150==0)
                {
                        vVerkehr[1]=vIdeal+1+random(3);
                        Verkehr[2]=1;
                        yVerkehr[1]=1000;
                        vIst = vVerkehr[1];
                        //Count1[1]=0;
                }
                if(Verkehr[3]!=1&&Count1[2]%150==0)
                {
                        vVerkehr[2]=Geschwindigkeit[0]-1-random(3);
                        Verkehr[3]=1;
                        yVerkehr[2]=0;
                }
                for(int i=0;i<3;i++)
                {
                        yVerkehr[i]+=Geschwindigkeit[0]-vVerkehr[i];
                        if(vVerkehr[i]!=0)
                        {
                                SetPen(Schwarz);
                                SetPenSize(4);
                                SetBrush(Weiss);
                                Rectangle(int(xSpur[i]),int(yVerkehr[i]),int(VerkehrBereich),int(VerkehrLaenge));
                        }
                        if(yVerkehr[i]>1100||yVerkehr[i]<-100)
                        {
                                Verkehr[i+1]=0;
                                Count1[i]++;
                        }

                }

                if(Geschwindigkeit[0]<vVerkehr[1]&&(nSpur[0]==1||nSpur[1]==1||nSpur[2]==1))
                {
                        Verkehr[2]=1;
                        if(yVerkehr[1]>(yLkw[2]+1.3*LkwLaenge)&&yVerkehr[1]<(yLkw[2]+1.6*LkwLaenge))
                                vVerkehr[1] = Geschwindigkeit[0]-1;
                        else
                                vVerkehr[1] = vIst;
                }
        }

        //====High Verkehr
        if(Verkehr[0]==2)
        {
                if(Verkehr[1]!=1&&Count1[0]%120==0)
                {
                        vVerkehr[0]=vMax-random(2);
                        Verkehr[1]=1;
                        yVerkehr[0]=1000;
                        yVerkehr[3]=yVerkehr[0]+3*VerkehrLaenge+30*random(15);
                }
                if(Verkehr[2]!=1&&Count1[1]%200==0)
                {
                        vVerkehr[1]=vIdeal+1+random(2);
                        Verkehr[2]=1;
                        yVerkehr[1]=1000;
                        yVerkehr[4]=yVerkehr[1]+2*VerkehrLaenge+30*random(15);
                        vIst = vVerkehr[1];
                        //Count1[1]=0;
                }
                if(Verkehr[3]!=1&&Count1[2]%200==0)
                {
                        vVerkehr[2]=Geschwindigkeit[0]-1-random(3);
                        Verkehr[3]=1;
                        yVerkehr[2]=0;
                        yVerkehr[5]=0-2*VerkehrLaenge-20*random(15);
                }
                for(int i=0;i<3;i++)
                {
                        yVerkehr[i]+=Geschwindigkeit[0]-vVerkehr[i];
                        yVerkehr[i+3]+=Geschwindigkeit[0]-vVerkehr[i];
                        if(vVerkehr[i]!=0)
                        {
                                SetPen(Schwarz);
                                SetPenSize(4);
                                SetBrush(Weiss);
                                Rectangle(int(xSpur[i]),int(yVerkehr[i]),int(VerkehrBereich),int(VerkehrLaenge));
                                Rectangle(int(xSpur[i]),int(yVerkehr[i+3]),int(VerkehrBereich),int(VerkehrLaenge));
                        }
                        if(yVerkehr[i]>1100||yVerkehr[i]<-200)
                        {
                                Verkehr[i+1]=0;
                                Count1[i]++;
                        }

                }

                if(Geschwindigkeit[0]<vVerkehr[1]&&(nSpur[0]==1||nSpur[1]==1||nSpur[2]==1))
                {
                        Verkehr[2]=1;
                        if((yVerkehr[1]>(yLkw[2]+1.2*LkwLaenge)&&yVerkehr[1]<(yLkw[2]+1.6*LkwLaenge))||(yVerkehr[1]>(yLkw[1]+1.2*LkwLaenge)&&yVerkehr[1]<(yLkw[1]+1.6*LkwLaenge)))
                        {
                                vVerkehr[1] = Geschwindigkeit[0]-1;
                                vVerkehr[4] = vVerkehr[1];
                        }
                        else if(Richtung[0]==0&&nSpur[0]!=1)
                        {
                                vVerkehr[1] = vIst;
                                vVerkehr[4] = vVerkehr[1];
                        }
                }
        }
}

//========================================================================
//= Funktionsname:
//=                Auto_Fahren()
//= Beschreibung:
//= Automatisierte Kolonnenfahrt
//========================================================================
void TUser::Auto_Fahren(void)
{
        if(Geschwindigkeit[0]>vVerkehr[nSpur[0]]&&(yLkw[0]-yVerkehr[nSpur[0]])<0.9*LkwLaenge&&yLkw[0]>yVerkehr[nSpur[0]])
        {
                Geschwindigkeit[0] -= 0.001*(yLkw[0]-yVerkehr[nSpur[0]]);
                Brems[0] = 1;
        }
        if(Geschwindigkeit[0]>vVerkehr[nSpur[0]]&&(yLkw[0]-yVerkehr[nSpur[0]+3])<0.9*LkwLaenge&&yLkw[0]>yVerkehr[nSpur[0]+3])
        {
                Geschwindigkeit[0] -= 0.001*(yLkw[0]-yVerkehr[nSpur[0]]);
                Brems[0] = 1;
        }
        else if(Geschwindigkeit[0]<=vVerkehr[nSpur[0]])
                Brems[0] = 0;

        //====low Verkehr
        if(AutoFahren==1&&Verkehr[0]==1)
        {
                if(Geschwindigkeit[0]<=vIdeal)
                       Geschwindigkeit[0]+=0.01*(vIdeal-Geschwindigkeit[0]);
                if(nSpur[0]==2&&Geschwindigkeit[0]<vIdeal&&(yLkw[0]>yVerkehr[nSpur[0]-1]+VerkehrLaenge)&&yLkw[0]>yVerkehr[nSpur[0]]&&yLkw[0]<(yVerkehr[nSpur[0]]+2*VerkehrLaenge))
                        Richtung[0] = -1;
                if(nSpur[0]!=2&&(yLkw[0]+1.3*LkwLaenge)<yVerkehr[2])
                        Richtung[0] = 1;
                SetTextSize(22);
                SetTextColor(Gruen);
                Text(755,100,"Auto Fahren");
                SetPenSize(3);
                SetPenColor(Gruen);
                SetBrush(Klar);
                Rectangle(752,102,102,22);
        }
        //==== High Verkehr
        else if(AutoFahren==1&&Verkehr[0]==2)
        {
                if(Geschwindigkeit[0]<=vIdeal)
                       Geschwindigkeit[0]+=0.01*(vIdeal-Geschwindigkeit[0]);
                if(nSpur[0]==2&&Geschwindigkeit[0]<vIdeal&&((yLkw[0]>yVerkehr[nSpur[0]+2]+VerkehrLaenge)&&(yLkw[0]>yVerkehr[nSpur[0]-1]+VerkehrLaenge))&&yLkw[0]>yVerkehr[nSpur[0]]&&yLkw[0]<(yVerkehr[nSpur[0]]+3*VerkehrLaenge))
                        Richtung[0] = -1;
                if(nSpur[0]!=2&&(yLkw[0]+1.3*LkwLaenge)<yVerkehr[2]&&(yLkw[0]+1.3*LkwLaenge)<yVerkehr[5])
                        Richtung[0] = 1;
                SetTextSize(22);
                SetTextColor(Gruen);
                Text(755,100,"Auto Fahren");
                SetPenSize(3);
                SetPenColor(Gruen);
                SetBrush(Klar);
                Rectangle(752,102,102,22);
        }
        else
        {
                View(750,95,110,30);
                Clear();
        }
}

//========================================================================
//= Funktionsname:
//=                Draw_Tempomesser()
//= Beschreibung:
//= Tachometer
//========================================================================
void TUser::Draw_Tempomesser(void)
{
        for(int i=0;i<3;i++)
        {
                 SetBrush(Weiss);
                SetTextSize(20);
                SetTextColor(Hellrot);
                Text(520,232+300*i,"0");
                Text(500,202+300*i,"20");
                Text(500,165+300*i,"40");
                Text(525,130+300*i,"60");
                Text(570,116+300*i,"80");
                Text(615,130+300*i,"100");
                Text(640,165+300*i,"120");
                Text(640,202+300*i,"140");
                Text(630,232+300*i,"160");
                SetPen(Hellrot);
                SetBrush(Hellrot);
                Circle(580,yLenkrad[i]-70,60,60);
                SetBrush(Schwarz);
                Circle(580,yLenkrad[i]-70,55,55);
                SetBrush(Hellrot);
                Circle(580,yLenkrad[i]-70,10,10);
                //Rectangle(577,yLenkrad[i]-70,6,-50);
                ClearPoints();
                SetPoint(577,yLenkrad[i]-70);
                SetPoint(583,yLenkrad[i]-70);
                SetPoint(int(580+50*cos(Geschwindigkeit[i]*0.265-3.93)),int(yLenkrad[i]-70+50*sin(Geschwindigkeit[i]*0.265-3.93)));
                Poly();
        }
        
}

//========================================================================
//= Funktionsname:
//=                Draw_Lenkrad()
//= Beschreibung:
//= Lenkrad
//========================================================================
void TUser::Draw_Lenkrad(void)
{
        for(int i=0;i<3;i++)
        {
                SetPen(Grau);
                SetBrush(Grau);
                Circle(580,yLenkrad[i],50,50);
                SetBrush(Weiss);
                Circle(580,yLenkrad[i],44,44);
                SetBrush(Grau);
                Circle(580,yLenkrad[i],15,15);
                if(Richtung[i]==0)
                {
                        Rectangle(532,yLenkrad[i]-3,96,6);
                        Rectangle(577,yLenkrad[i]-3,6,47);
                }
                else
                {
                        ClearPoints();
                        SetPoint(580+Richtung[i]*33,yLenkrad[i]+37);
                        SetPoint(580+Richtung[i]*37,yLenkrad[i]+33);
                        SetPoint(580-Richtung[i]*33,yLenkrad[i]-37);
                        SetPoint(580-Richtung[i]*37,yLenkrad[i]-33);
                        Poly();
                        ClearPoints();
                        SetPoint(580+Richtung[i]*2,yLenkrad[i]+2);
                        SetPoint(580-Richtung[i]*2,yLenkrad[i]-2);
                        SetPoint(580-Richtung[i]*37,yLenkrad[i]+33);
                        SetPoint(580-Richtung[i]*33,yLenkrad[i]+37);
                        Poly();
                }
        }
}

//========================================================================
//= Funktionsname:
//=                Draw_Spurassistent()
//= Beschreibung:
//= Spurwechselassistent
//========================================================================
void TUser::Draw_Spurassistent(void)
{
        for(int i=0;i<3;i++)
        {
                if(((yVerkehr[nSpur[i]-1]-yLkw[i])<1.2*LkwLaenge&&yVerkehr[nSpur[i]-1]>yLkw[i])||((yVerkehr[nSpur[i]+2]-yLkw[i])<1.2*LkwLaenge&&yVerkehr[nSpur[i]+2]>yLkw[i]))
                {

                        SetPen(Hellrot);
                        SetBrush(Hellrot);
                        ClearPoints();
                        SetPoint(420,300+300*i);
                        SetPoint(490,300+300*i);
                        SetPoint(455,239+300*i);
                        Poly();

                        SetPen(Weiss);
                        SetBrush(Weiss);
                        ClearPoints();
                        SetPoint(428,296+300*i);
                        SetPoint(482,296+300*i);
                        SetPoint(455,248+300*i);
                        Poly();

                        SetBrush(Hellrot);
                        Rectangle(452,260+300*i,6,26);
                        Rectangle(452,287+300*i,6,7);
                }
                if(((yVerkehr[nSpur[i]+1]-yLkw[i])<1.2*LkwLaenge&&yVerkehr[nSpur[i]+1]>yLkw[i])||((yVerkehr[nSpur[i]+4]-yLkw[i])<1.2*LkwLaenge&&yVerkehr[nSpur[i]+4]>yLkw[i]))
                {
                        SetPen(Hellrot);
                        SetBrush(Hellrot);
                        ClearPoints();
                        SetPoint(660,300+300*i);
                        SetPoint(730,300+300*i);
                        SetPoint(695,239+300*i);
                        Poly();

                        SetPen(Weiss);
                        SetBrush(Weiss);
                        ClearPoints();
                        SetPoint(668,296+300*i);
                        SetPoint(722,296+300*i);
                        SetPoint(695,248+300*i);
                        Poly();

                        SetBrush(Hellrot);
                        Rectangle(692,260+300*i,6,26);
                        Rectangle(692,287+300*i,6,7);
                }
        }
}

//========================================================================
//= Funktionsname:
//=                Draw_HUD()
//= Beschreibung:
//= Fahrzustand
//========================================================================
void TUser::Draw_HUD(void)
{
        View(400,10,350,1000);
        Clear();
        SetTextSize(30);
        SetBrush(Weiss);
        Text(550,330,"LKW 1");
        Text(550,630,"LKW 2");
        Text(550,930,"LKW 3");
        Draw_Tempomesser();
        Draw_Lenkrad();
        Draw_Spurassistent();
}
//========================================================================
//= Funktionsname:
//=                Sensordaten ()
//= Beschreibung:
//= Punktmessung von Sensor
//========================================================================
void TUser::Sensordaten(void)
{
//====Frontradar LKW1
        for (int i=0;i<5;i++)
        {
             //====LKW0Unter LKW0
             if(int(xLkw[0]+0.25*i*LkwBereich)>=int(xLkw[1]+0.5*LkwBereich)-0.577*(int(yLkw[1]-yLkw[0]-LkwLaenge))
             &&int(xLkw[0]+0.25*i*LkwBereich)<=int(xLkw[1]+0.5*LkwBereich)+0.577*(int(yLkw[1]-yLkw[0]-LkwLaenge))
             &&int(yLkw[1]-yLkw[0]-LkwLaenge)<=150&&int(yLkw[1]-yLkw[0]-LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[0]+0.25*i*LkwBereich),int(yLkw[0]+LkwLaenge),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]));
                 LineTo(int(xLkw[0]+0.25*i*LkwBereich),int(yLkw[0]+LkwLaenge));
             }
        }
        for (int i=0;i<9;i++)
        {
             //====LKW0Rechts LKW0
             if(int(xLkw[0])>=int(xLkw[1]+0.5*LkwBereich)&&
             int(xLkw[0]-xLkw[1]-0.5*LkwBereich)<=0.577*(int(yLkw[1]-yLkw[0]-LkwLaenge))
             &&int(yLkw[1]-yLkw[0]-0.125*i*LkwLaenge)<=150&&int(yLkw[1]-yLkw[0]-0.125*i*LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[0]),int(yLkw[0]+0.125*i*LkwLaenge),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]));
                 LineTo(int(xLkw[0]),int(yLkw[0]+0.125*i*LkwLaenge));
             }
             //====LKW0Links LKW0
             if(int(xLkw[0]+LkwBereich)<int(xLkw[1]+0.5*LkwBereich)&&
             int(xLkw[1]+0.5*LkwBereich-xLkw[0]-LkwBereich)<=0.577*(int(yLkw[1]-yLkw[0]-LkwLaenge))
             &&int(yLkw[1]-yLkw[0]-0.125*i*LkwLaenge)<=150&&int(yLkw[1]-yLkw[0]-0.125*i*LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[0]+LkwBereich),int(yLkw[0]+0.125*i*LkwLaenge),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]));
                 LineTo(int(xLkw[0]+LkwBereich),int(yLkw[0]+0.125*i*LkwLaenge));
             }
        }
//====Heckradar LKW1
        for (int i=0;i<5;i++)
        {
             //====LKW2Ober LKW2
             if(int(xLkw[2]+0.25*i*LkwBereich)>=int(xLkw[1]+0.5*LkwBereich)-int(yLkw[2]-yLkw[1]-LkwLaenge)
             &&int(xLkw[2]+0.25*i*LkwBereich)<=int(xLkw[1]+0.5*LkwBereich)+int(yLkw[2]-yLkw[1]-LkwLaenge)
             &&int(yLkw[2]-yLkw[1]-LkwLaenge)<=80&&int(yLkw[2]-yLkw[1]-LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[2]+0.25*i*LkwBereich),int(yLkw[2]),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge));
                 LineTo(int(xLkw[2]+0.25*i*LkwBereich),int(yLkw[2]));
             }
        }
        for (int i=0;i<5;i++)
        {
             //====LKW2Rechts LKW2
             if(int(xLkw[2])>=int(xLkw[1]+0.5*LkwBereich)&&
             int(xLkw[2]-xLkw[1]-0.5*LkwBereich)<=int(yLkw[2]-yLkw[1]-LkwLaenge)
             &&int(yLkw[2]+0.1*i*LkwLaenge-yLkw[1]-LkwLaenge)<=80&&int(yLkw[2]-yLkw[1]-0.1*i*LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[2]),int(yLkw[2]+0.1*i*LkwLaenge),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge));
                 LineTo(int(xLkw[2]),int(yLkw[2]+0.1*i*LkwLaenge));
             }
             //====LKW2Links LKW2
             if(int(xLkw[2]+LkwBereich)<int(xLkw[1]+0.5*LkwBereich)&&
             int(xLkw[1]+0.5*LkwBereich-xLkw[2]-LkwBereich)<=int(yLkw[2]-yLkw[1]-LkwLaenge)
             &&int(yLkw[2]+0.1*i*LkwLaenge-yLkw[1]-LkwLaenge)<=80&&int(yLkw[2]-yLkw[1]-0.1*i*LkwLaenge)>=0)
             {
                 SetPen(Hellgruen);
                 SetBrush(Klar);
                 Circle(int(xLkw[2]+LkwBereich),int(yLkw[2]+0.1*i*LkwLaenge),4);
                 SetPen(Hellgruen);
                 MoveTo(int(xLkw[1]+0.5*LkwBereich),int(yLkw[1]+LkwLaenge));
                 LineTo(int(xLkw[2]+LkwBereich),int(yLkw[2]+0.1*i*LkwLaenge));
             }
        }
//====Seitenradar LKW1
        for(int i=0;i<3;i++)
        {
             if(vVerkehr[i]!=0)
             {
                  //====Rechtsradar LKW1
                  for(int i=0;i<9;i++)
                  {
                      //====Verkehr[2]Links
                      if(int (xSpur[2]-xLkw[1]-LkwBereich)>0&&int (xSpur[2]-xLkw[1]-LkwBereich)<100
                      &&int (yVerkehr[2]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xSpur[2]-xLkw[1]-LkwBereich)&&int (yVerkehr[2]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xSpur[2]-xLkw[1]-LkwBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]),int(yVerkehr[2]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]),int(yVerkehr[2]+0.125*i*VerkehrLaenge));
                      }
                  }
                  for(int i=0;i<9;i++)
                  {
                      //====Verkehr[5]Links
                      if(int (xSpur[2]-xLkw[1]-LkwBereich)>0&&int (xSpur[2]-xLkw[1]-LkwBereich)<100
                      &&int (yVerkehr[5]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xSpur[2]-xLkw[1]-LkwBereich)&&int (yVerkehr[5]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xSpur[2]-xLkw[1]-LkwBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]),int(yVerkehr[5]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]),int(yVerkehr[5]+0.125*i*VerkehrLaenge));
                      }
                  }
                  for(int i=0;i<5;i++)
                  {
                      //====Verkehr[2]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[2]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[2]-VerkehrLaenge)<=1.732*int (xSpur[2]-xLkw[1]-LkwBereich)
                      &&int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)<=100&&
                      int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[2]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[2]+VerkehrLaenge));
                      }
                      //====Verkehr[2]Ober
                      if(int (yVerkehr[2]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[2]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xSpur[2]-xLkw[1]-LkwBereich)
                      &&int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)<=100&&
                      int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[2]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[2]));
                      }
                  }
                  for(int i=0;i<5;i++)
                  {
                      //====Verkehr[5]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[5]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[5]-VerkehrLaenge)<=1.732*int (xSpur[2]-xLkw[1]-LkwBereich)
                      &&int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)<=100&&
                      int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[5]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[5]+VerkehrLaenge));
                      }
                      //====Verkehr[5]Ober
                      if(int (yVerkehr[5]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[5]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xSpur[2]-xLkw[1]-LkwBereich)
                      &&int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)<=100&&
                      int (xSpur[2]+0.25*i*VerkehrBereich-xLkw[1]-LkwBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[5]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]+LkwBereich),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[2]+0.25*i*VerkehrBereich),int(yVerkehr[5]));
                      }
                  }
                  //====Linksradar LKW1
                  for(int i=0;i<9;i++)
                  {
                      //====Verkehr[1]Rechts
                      if(int (xLkw[1]-xSpur[1]-VerkehrBereich)>0&&int (xLkw[1]-xSpur[1]-VerkehrBereich)<100
                      &&int (yVerkehr[1]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)&&int (yVerkehr[1]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+VerkehrBereich),int(yVerkehr[1]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+VerkehrBereich),int(yVerkehr[1]+0.125*i*VerkehrLaenge));
                      }
                  }
                  for(int i=0;i<9;i++)
                  {
                      //====Verkehr[4]Rechts
                      if(int (xLkw[1]-xSpur[1]-VerkehrBereich)>0&&int (xLkw[1]-xSpur[1]-VerkehrBereich)<100
                      &&int (yVerkehr[4]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)&&int (yVerkehr[4]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+VerkehrBereich),int(yVerkehr[4]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+VerkehrBereich),int(yVerkehr[4]+0.125*i*VerkehrLaenge));
                      }
                  }
                   for(int i=0;i<5;i++)
                  {
                      //====Verkehr[1]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[1]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[1]-VerkehrLaenge)<=1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[1]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[1]+VerkehrLaenge));
                      }
                      //====Verkehr[1]Ober
                      if(int (yVerkehr[1]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[1]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[1]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[1]));
                      }
                  }
                   for(int i=0;i<5;i++)
                  {
                      //====Verkehr[4]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[4]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[4]-VerkehrLaenge)<=1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[4]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[4]+VerkehrLaenge));
                      }
                      //====Verkehr[1]Ober
                      if(int (yVerkehr[4]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[4]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xLkw[1]-xSpur[1]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[1]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[4]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[1]+0.25*i*VerkehrBereich),int(yVerkehr[4]));
                      }
                  }
                  for(int i=0;i<9;i++)
                  {
                      //====Verkehr[0]Rechts
                      if(int (xLkw[1]-xSpur[0]-VerkehrBereich)>0&&int (xLkw[1]-xSpur[0]-VerkehrBereich)<100
                      &&int (yVerkehr[0]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)&&int (yVerkehr[0]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+VerkehrBereich),int(yVerkehr[0]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+VerkehrBereich),int(yVerkehr[0]+0.125*i*VerkehrLaenge));
                      }
                  }
                   for(int i=0;i<9;i++)
                  {
                      //====Verkehr[3]Rechts
                      if(int (xLkw[1]-xSpur[0]-VerkehrBereich)>0&&int (xLkw[1]-xSpur[0]-VerkehrBereich)<100
                      &&int (yVerkehr[3]+0.125*i*VerkehrLaenge)>=int (yLkw[1]+0.5*LkwLaenge)
                      -1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)&&int (yVerkehr[3]+0.125*i*VerkehrLaenge)
                      <=int (yLkw[1]+0.5*LkwLaenge)+1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich))
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+VerkehrBereich),int(yVerkehr[3]+0.125*i*VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+VerkehrBereich),int(yVerkehr[3]+0.125*i*VerkehrLaenge));
                      }
                  }
                  for(int i=0;i<5;i++)
                  {
                      //====Verkehr[0]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[0]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[0]-VerkehrLaenge)<=1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[0]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[0]+VerkehrLaenge));
                      }
                      //====Verkehr[0]Ober
                      if(int (yVerkehr[0]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[0]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[0]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[0]));
                      }
                  }
                  for(int i=0;i<5;i++)
                  {
                      //====Verkehr[3]Unter
                      if(int (yLkw[1]+0.5*LkwLaenge-yVerkehr[3]-VerkehrLaenge)>=0&&
                      int (yLkw[1]+0.5*LkwLaenge-yVerkehr[3]-VerkehrLaenge)<=1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[3]+VerkehrLaenge),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[3]+VerkehrLaenge));
                      }
                      //====Verkehr[3]Ober
                      if(int (yVerkehr[3]-yLkw[1]-0.5*LkwLaenge)>=0&&
                      int (yVerkehr[3]-yLkw[1]-0.5*LkwLaenge)<=1.732*int (xLkw[1]-xSpur[0]-VerkehrBereich)
                      &&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)<=100&&int (xLkw[1]-xSpur[0]-0.25*i*VerkehrBereich)>=0)
                      {
                           SetPen(Hellgruen);
                           SetBrush(Klar);
                           Circle(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[3]),4);
                           SetPen(Hellgruen);
                           MoveTo(int(xLkw[1]),int(yLkw[1]+0.5*LkwLaenge));
                           LineTo(int(xSpur[0]+0.25*i*VerkehrBereich),int(yVerkehr[3]));
                      }
                  }
             }
        }
}
//========================================================================
//= Funktionsname:
//=                Draw_Tagwechsel()
//= Beschreibung:
//= Symbol Tag und Nacht
//========================================================================
void TUser::Draw_Tagwechsel(void)
{
        View(400,10,400,90);
        Clear(Weiss);
        SetPen(Hellrot,10);
        MoveTo(500,50);
        LineTo(530,50);
        MoveTo(500,50);
        LineTo(515,24);
        MoveTo(500,50);
        LineTo(485,24);
        MoveTo(500,50);
        LineTo(470,50);
        MoveTo(500,50);
        LineTo(485,76);
        MoveTo(500,50);
        LineTo(515,76);
        SetPen(Gelb,8);
        SetBrush(Hellgelb);
        Circle(500,50,20);
        SetPen(Weiss,4);
        SetBrush(Klar);
        Circle(500,50,25);
        SetPen(Schwarz);
        SetBrush(Schwarz);
        Rectangle(620,20,60,60);
        SetPen(Gelb,3);
        SetBrush(Hellgelb);
        Circle(650,50,30);
        SetBrush(Schwarz);
        Circle(620,50,42);
        SetPen(Weiss);
        SetBrush(Weiss);
        Rectangle(620,20,-60,-60);
        Rectangle(620,20,160,-60);
        Rectangle(620,20,-60,60);
        Rectangle(580,80,100,20);
        ClearPoints();
        SetPoint(635,20);
        SetPoint(638,32);
        SetPoint(650,35);
        SetPoint(638,38);
        SetPoint(635,50);
        SetPoint(632,38);
        SetPoint(620,35);
        SetPoint(632,32);
        SetPenColor(Gelb);
        SetBrushColor(Hellgelb);
        Poly();

}

//========================================================================
//= Funktionsname:
//=                Draw_Pedal()
//= Beschreibung:
//= Brems- und Gaspedal
//========================================================================
void TUser::Draw_Pedal(void)
{
        View(445,10,400,1000);
        Clear(Klar);
        SetText(Schwarz,18);
        SetBrush(Klar);
        Text(445,120,"Bremse");
        Text(670,120,"  Gas");
        SetPen(Schwarz,3);
        SetBrush(Hellgrau);
        Rectangle(450,140,35,80);
        Rectangle(675,140,35,80);
        for(int i=0;i<6;i++)
        {
                SetPen(Schwarz,2);
                MoveTo(450+7*i,140);
                LineTo(450+7*i,220);
                MoveTo(675+7*i,140);
                LineTo(675+7*i,220);
        }
}
//========================================================================
//= Funktionsname:
//=                BildMouseDown()
//= Beschreibung:
//= Mausklick
//========================================================================
void TUser::BildMouseDown(int x, int y)
{
        if(AutoFahren!=1)
        {
                if((x>=530)&&(x<=580)&&(y>=220)&&(y<=320)&&nSpur[0]>0) {
                Richtung[0] = -1;
                }
                if((x>=580)&&(x<=630)&&(y>=220)&&(y<=320)&&nSpur[0]<2) {
                Richtung[0] = 1;
                }
        }
        if((x>=620)&&(x<=680)&&(y>=20)&&(y<=80)) {
        Tag = -1;
        }
        if((x>=470)&&(x<=530)&&(y>=20)&&(y<=80)) {
        Tag = 1;
        }

}
//========================================================================
//= Funktionsname:
//=                BildMouseMove()
//= Beschreibung:
//= Mausbewegung
//========================================================================
void TUser::BildMouseMove(int x, int y, int leftdown)
{
        if((x>=450)&&(x<=485)&&(y>=140)&&(y<=220)&&(leftdown)&&Geschwindigkeit[0]>0){
            Geschwindigkeit[0]-=0.02;
        }
        if((x>=675)&&(x<=710)&&(y>=140)&&(y<=220)&&(leftdown)&&Geschwindigkeit[0]<vMax) {
            Geschwindigkeit[0]+=0.02;
        }
}

//========================================================================
//= Funktionsname:
//=                V_Anzeige()
//= Beschreibung:
//= Geschwindigkeit
//========================================================================
  void TUser::V_Anzeige(void){
        View(1250,80,350,20);
        Clear(Weiss);
        BrushColor = Klar;
        SetPen(Schwarz);
        SetTextSize(15);
        char ss[100];
        sprintf(ss,"mittlere Geschwindigkeit v = %.1f km/h",3.0*(Geschwindigkeit[0]+Geschwindigkeit[1]+Geschwindigkeit[2]));
        Text(1250,80,ss);
        }
//========================================================================
//= Funktionsname:
//=                KSV_Anzeige()
//= Beschreibung:
//= Kraftstoffverbrauch
//========================================================================
void TUser::KSV_Anzeige(void){
        View(1250,480,350,20);
        Clear(Weiss);
        BrushColor = Klar;
        SetPen(Schwarz);
        SetTextSize(15);
        char ss[100];
        sprintf(ss,"mittlerer Kraftstoffverbauch Q_L = %.1f l/100km",5.13+0.0014636*9.0
        *(Geschwindigkeit[0]+Geschwindigkeit[1]+Geschwindigkeit[2])*
        (Geschwindigkeit[0]+Geschwindigkeit[1]+Geschwindigkeit[2]));
        Text(1250,480,ss);
        }
//========================================================================

//========================================================================
//= Funktionsname:
//=                Plot_Diagramm()
//= Beschreibung:
//= Plotgrafiken
//========================================================================
void TUser::Plot_Diagramm(void)
{
        Plot0->Size(900,100,700,350);
        Plot0->Titel = "Geschwindigkeit";
        Plot0->AchsenTyp = 0;
        Plot0->Achse(0,0,CallRunCount);
        Plot0->Achse(1,0,150);
        Plot0->Achse(0,"Zeit [s]");
        Plot0->Achse(1,"v [ KM / h ]");
        Plot0->Kurve0->SetPen(Hellrot,3);
        Plot0->Kurve1->SetPen(Gruen,3);
        Plot0->Kurve2->SetPen(Blau,3);

//=========================================================================
        Plot1->Size(900,500,700,350);
        Plot1->Titel = "Dieselverbrauch";
        Plot1->AchsenTyp = 0;
        Plot1->Achse(1,0,35);
        Plot1->Achse(0,0,CallRunCount);
        Plot1->Achse(0,"Zeit [s]");
        Plot1->Achse(1,"Q_L [ l / 100 KM ]");
        Plot1->Kurve0->SetPen(Hellrot,3);
        Plot1->Kurve1->SetPen(Gruen,3);
        Plot1->Kurve2->SetPen(Blau,3);
}

void TUser::Dynamik_Plot(void)
{
        //Plot0->MoveAchse(0);
        Plot0->Kurve0->LineTo(0.02*CallRunCount,Geschwindigkeit[0]*9);
        Plot0->Kurve0->MoveTo(0.02*CallRunCount,Geschwindigkeit[0]*9);
        Plot0->Kurve1->LineTo(0.02*CallRunCount,Geschwindigkeit[1]*9);
        Plot0->Kurve1->MoveTo(0.02*CallRunCount,Geschwindigkeit[1]*9);
        Plot0->Kurve2->LineTo(0.02*CallRunCount,Geschwindigkeit[2]*9);
        Plot0->Kurve2->MoveTo(0.02*CallRunCount,Geschwindigkeit[2]*9);
        Plot0->Kurve2->SetPen(Blau,3);
        if(!Plot0->Kurve0->LineTo(0.02*CallRunCount,Geschwindigkeit[0]*9)) {
            Plot0->MoveAchse(0);
            Plot1->MoveAchse(0);
        }
//========================================================================
        //Plot1->MoveAchse(0);
        Plot1->Kurve0->LineTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[0]*Geschwindigkeit[0]);
        Plot1->Kurve0->MoveTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[0]*Geschwindigkeit[0]);
        //if(9*Geschwindigkeit[0]<=20){Plot1->Kurve0->MoveTo(0.02*CallRunCount,0.286*9*Geschwindigkeit[0]);
        //}
        Plot1->Kurve1->LineTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[1]*Geschwindigkeit[1]);
        Plot1->Kurve1->MoveTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[1]*Geschwindigkeit[1]);
        Plot1->Kurve2->LineTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[2]*Geschwindigkeit[2]);
        Plot1->Kurve2->MoveTo(0.02*CallRunCount,5.13+
        0.0014636*81*Geschwindigkeit[2]*Geschwindigkeit[2]);
}
//==== Polizeikontrolle
void TUser::Draw_Polizei(void)
{
        SetPenColor(Schwarz);
        SetBrushColor(Weiss);
        Rectangle(int(xLkw[0]),int(yLkw[0]-LkwLaenge),int(VerkehrBereich),int(VerkehrLaenge));
        SetBrushColor(Hellblau);
        if(t%30<=20)
        {
                Rectangle(int(xLkw[0]),int(yLkw[0]-LkwLaenge+0.5*VerkehrLaenge),int(3*LinieBereich),int(3*LinieBereich));
                Rectangle(int(xLkw[0]+VerkehrBereich),int(yLkw[0]-LkwLaenge+0.5*VerkehrLaenge),-int(3*LinieBereich),int(3*LinieBereich));
        }
        else if(t%30>=10)
        {
                SetBrushColor(Hellrot);
                Rectangle(int(xLkw[0]+int(3*LinieBereich)),int(yLkw[0]-LkwLaenge+0.5*VerkehrLaenge),int(4*LinieBereich),int(3*LinieBereich));
        }
        for(int i=0;i<3;i++)
        {
                if(t%20<10)
                {
                        Draw_Blinker(i,1);
                        Draw_Blinker(i,-1);
                }
        }
        Verkehr[0]=0;
        AutoFahren=-1;
        if(Geschwindigkeit[0]>0)
                Geschwindigkeit[0]-=0.02;
        else
                Geschwindigkeit[0]=0;
        if(Geschwindigkeit[0]<7&&xLkw[0]==xSpur[2])
                Richtung[0]=1;
}

//========================================================================
//= Funktionsname:
//=                Init()
//= Beschreibung:
//= Init
//========================================================================
void TUser::Init(void)       
{
        ProgrammName = "KlonnenFahren";
        ProgrammInfo = "Programm zur Vorlesung:\r"
                        "Simulation komplexer Systeme\r"
                        "// Projektteilnehmer:
                        // Name		      Matrikel-Nr.
                        // Shi, Ran		4825188
                        // Gao, Pengxiao	4872458
                        // Wang, Kai		4934820";
        Init_BasicDaten();
        TastenfeldName = "Geschwindigkeit";
        InsertTaste(0,"Speed +");
        InsertTaste(1,"Speed -");
        InsertTaste(2,"Links");
        InsertTaste(3,"Rechts");
        InsertTaste(6,"Verkehr");
        Draw_Spur();
        Draw_Auto();
        Draw_HUD();
        Sensordaten();
        Draw_Tagwechsel();
        Draw_Pedal();
        V_Anzeige();
        KSV_Anzeige();
        Plot_Diagramm();
        Plot0->Achse(0,0,30);
        Plot1->Achse(0,0,30);
        Hauptmenu();
}

//========================================================================
//= Funktionsname:
//=                Run()
//= Beschreibung:
//= Run
//========================================================================
void TUser::Run(void)
{
        Clear();
        Berechnen_Abmessung();
        Berechnen_Richtung();
        Auto_Fahren();
        Draw_Spur();
        Draw_Auto();
        Draw_Verkehr();
        Draw_HUD();
        Draw_Tagwechsel();
        Draw_Pedal();
        V_Anzeige();
        KSV_Anzeige();
        
        CallRun = true;
        Dynamik_Plot();
        t++;
        
}


//==== Reset

void TUser::Reset(void)
{
        Init();
        V_Anzeige();
        Plot0->Reset();
        KSV_Anzeige();
        Plot1->Reset();
}

//========================================================================
//= Beschreibung:
//= Usertaste
//========================================================================
//====
void TUser::Hauptmenu(void)
{
        Menu = 0;
        DeleteAllTasten();
        TastenfeldName = "Hauptmen��";
        InsertTaste(0,"Manuell.","Men��zum Einstellen des Manuellskontroller");
        InsertTaste(1,"Automatisch.","Men��zum Einstellen des Automatisches Fahrens");
        InsertTaste(4,"Speziell","Men��zum Einstellen des Speziellzustand");
        InsertTaste(5,"Parameter","Men��zum Einstellen des Parameter");
        InsertTaste(8,"Reserviert");
        InsertTaste(9,"Reserviert");
}



void TUser::Manuellsmenu(void)
{
        Menu = 1;
        DeleteAllTasten();
        TastenfeldName = "Manuell Controller";
        InsertTaste(0,"Speed +");
        InsertTaste(1,"Speed -");
        InsertTaste(4,"Left");
        InsertTaste(5,"Right");
        InsertTaste(13,"Back");
}

//====
void TUser::AutoFahrensmenu(void)
{
        Menu = 2;
        DeleteAllTasten();
        TastenfeldName = "Automatisches Fahren";
        InsertTaste(0,"NiedigFluss");
        InsertTaste(1,"HochFluss");
        InsertTaste(4,"Fahren On/off");
        InsertTaste(5,"Radar On/Off");
        InsertTaste(13,"Back");
}

void TUser::Speziellsmenu(void)
{
        Menu = 3;
        DeleteAllTasten();
        TastenfeldName = "Speziellszustand";
        InsertTaste(0,"L.geschlossen");
        InsertTaste(1,"M.geschlossen");
        InsertTaste(2,"R.geschlossen");
        InsertTaste(6,"Polizei Kontorller");
        InsertTaste(13,"Back");

}

void TUser::Parametersmenu(void)
{
        Menu = 4;
        DeleteAllTasten();
        TastenfeldName = "Parameterveranderung";

        InsertTaste(13,"Back");

}

void TUser::RunTaste0(void)
{
        switch(Menu)
        {
                case 0: Manuellsmenu();break;
                case 1:
                        if(Geschwindigkeit[0]<vMax)
                        Geschwindigkeit[0]+=1;
                        break;
                case 2:
                        if(Verkehr[0]!=1)
                                Verkehr[0]=1;
                        else
                                Verkehr[0]=0;
                        yVerkehr[0] = -100;
                        yVerkehr[1] = -100;
                        yVerkehr[2] = -100;
                        Count1[0]=Count1[1]=Count1[2]=0;
                        break;
                case 3:
                        Bau[0]=-Bau[0];

        }
}


void TUser::RunTaste1(void)
{
        switch(Menu)
        {
                case 0: AutoFahrensmenu();break;
                case 1:
                        if(Geschwindigkeit[0]>0)
                        Geschwindigkeit[0]-=1;
                        break;
                case 2:
                        if(Verkehr[0]!=2)
                                Verkehr[0]=2;
                        else
                                Verkehr[0]=0;
                        yVerkehr[0] = -100;
                        yVerkehr[1] = -100;
                        yVerkehr[2] = -100;
                        yVerkehr[3] = -100;
                        yVerkehr[4] = -100;
                        yVerkehr[5] = -100;
                        Count1[0]=Count1[1]=Count1[2]=0;
                        break;
                case 3:
                        Bau[1]=-Bau[1];
        }
}

void TUser::RunTaste2(void)
{
        Bau[2]=-Bau[2];
}
void TUser::RunTaste4(void)
{
        switch(Menu)
        {
                case 0: Speziellsmenu();
                        Init_BasicDaten();
                        break;
                case 1:
                if(xLkw[0]>xSpur[0]&&Verkehr[0]==0)
                        Richtung[0] = -1;
                        else    Richtung[0] = 0;
                        break;
                case 2:
                        AutoFahren = -AutoFahren;
                        break;
        }
}

void TUser::RunTaste5(void)
{
        switch(Menu)
        {
                case 0: Parametersmenu();break;
                case 1:
                        if(xLkw[0]<xSpur[2]&&Verkehr[0]==0)
                        Richtung[0] = 1;
                        else    Richtung[0] = 0;
                        break;
                case 2: RadarOn = -RadarOn;
                        break;
        }
}

void TUser::RunTaste6(void)
{
        Polizei=1;
}

void TUser::RunTaste13(void)
{
        Hauptmenu();
}

// 4. main
//====================================================================
#pragma argsused
int main(int argc, char* argv[]){ TUser a; a.Execute(); return 0; }
//__________________________________________________________Ost08_____


