unit u3DSpline;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
  uPositionList,
  DGLOpenGL;

type
  //Dynamisches Array aus Single-Werten
  TSingleArray = array of single;

  //Record mit Daten der Spline an einer bestimmten Stelle
  TSplineData = packed record
    //Position im Raum
    //|  Drehwinkel der Achsen
    Pos, Angle: TGLVector3f;
  end;

  //Kubisches Spline, welches Punkte im Raum interpoliert
  T3DSpline = class(TPositionList)
  private
    //Parameter der kubischen Funktionen
    fSpline: array[0..2] of TSingleArray;
    //Länge der Spline in Weltkoordinaten
    fLength: Single;
    function GetLength: Single;
    procedure CreateCubicSpline(const y: TSingleArray; var K: TSingleArray);
    function GetCubicSplineAt(const K: TSingleArray; t: single): single;
  public
    //Eigenschaften
    property SplineLength: Single read fLength;
    //Methoden
    procedure GenSpline;
    function GetSplineAt(const t: Single): TglVector3f;
    function GetSplineDataAt(t, next: Single): TSplineData;
    constructor Create;
    destructor Destroy; override;
  end;

implementation

uses
  Math;

function glVector3f(X, Y, Z: Single): TglVector3f;
begin
  result[0] := X;
  result[1] := Y;
  result[2] := Z;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//T3DSpline/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRI//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//berechnet die Länge der Spline
//@result: Länge der Spline in WeltKoordinaten;
function T3DSpline.GetLength: Single;
var
  i, max: Integer;
  P1, P2: TGLVector3f;
begin
  max := 100 * fList.Count;
  P1 := glVector3f(0,0,0);
  result := 0;
  for i := 0 to max do begin
    P2 := P1;
    P1 := GetSplineAt(i/max);
    result := result + SQRT(SQR(P1[0]-P2[0])+SQR(P1[1]-P2[1])+SQR(P1[2]-P2[2]));
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt eine Kubische Splie aus den übergebenen  Werten
//@y: Strützpunkte der Spline;
//@k: Parameter der Splinefunktion;
procedure T3DSpline.CreateCubicSpline(const y: TSingleArray; var K: TSingleArray);
var
  n, i: Integer;
  r, u, t: TSingleArray;
  b: single;
begin
  n := Length(y);
  case n of
    0: SetLength(K, 0);
    1: begin
      SetLength(K, 1);
      K[0] := y[0];
    end;
    2: begin
      SetLength(K, 2);
      K[0] := y[0];
      K[1] := y[1] - y[0];
    end;
    else begin
      n := Length(y) - 1;

      SetLength(r, n);
      SetLength(u, n - 1);
      SetLength(t, n - 1);
      SetLength(K, 4 * n + 2);

      for i := 0 to n - 2 do
        r[i] := 3 * (y[i + 2] - 2 * y[i + 1] + y[i]);
      b      := 4;
      u[0]   := r[0] / b;
      for i := 1 to n - 2 do begin
        t[i] := 1 / b;
        b    := 4 - t[i];
        u[i] := (r[i] - u[i - 1]) / b;
      end;
      for i := n - 3 downto 0 do
        u[i] := u[i] - t[i + 1] * u[i + 1];
      for i := 0 to n - 1 do
        K[i * 4 + 0] := y[i + 1];
      for i := 0 to n - 2 do
        K[i * 4 + 2] := u[i];
      K[3] := K[2] / 3;
      for i := 1 to n - 1 do
        K[i * 4 + 3] := (K[i * 4 + 2] - K[(i - 1) * 4 + 2]) / 3;
      K[1] := y[1] - y[0] + 2 / 3 * K[2];
      for i := 1 to n - 1 do
        K[i * 4 + 1] := y[i + 1] - Y[i] + (2 * K[i * 4 + 2] + K[(i - 1) * 4 + 2]) / 3;

      K[High(K) - 1] := (GetCubicSplineAt(K, 0.01) - GetCubicSplineAt(K, 0.00))*100;
      K[High(K)]     := (GetCubicSplineAt(K, 1.00) - GetCubicSplineAt(K, 0.99))*100;
    end;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt einen Punkt auf der Spline
//@K: Parameter der Spline;
//@t: Position auf der Spline (zwischen 0 und 1);
//@result: Wert der Spline mit den Parametern K an der Stelle t;
function T3DSpline.GetCubicSplineAt(const K: TSingleArray; t: single): Single;
var
  n, i: integer;
  p: Single;
begin
  n := Length(K);
  case n of
    0: exit(0);
    1: exit(K[0]);
    2: exit(K[0] + t * K[1]);
    else
    begin
      if t < 0 then begin
        exit(GetCubicSplineAt(K, 0) + K[High(K) - 1] * t);
      end
      else if t > 1 then begin
        exit(GetCubicSplineAt(K, 1) - K[High(K)] * (1 - t));
      end else begin
        n := (Length(K)-2) div 4;
        t := t * n;
        i := Trunc(t);
        if i >= n then
          dec(i);
        p := t-i-1;
        exit(K[i*4] + (K[i*4+1] + (K[i*4+2] + K[i*4+3]*p)*p)*p);
      end;
    end;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt die Spline aus dem Daten der Stützpunkte
procedure T3DSpline.GenSpline;
var
  i, j: Integer;
  y: TSingleArray;
begin
  SetLength(y, fList.Count);
  for j := 0 to 2 do
    for i := 0 to fList.Count-1 do begin
      y[i] := PglVector3f(fList[i])^[j];
      CreateCubicSpline(y, fSpline[j]);
    end;
  fLength := GetLength;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittel die Position auf der Spline
//@t: Stelle auf der Spline (zwischen 0 und 1);
//@result: Position auf der Spline;
function T3DSpline.GetSplineAt(const t: Single): TglVector3f;
var i: Integer;
begin
  for i := 0 to 2 do
    result[i] := GetCubicSplineAt(fSpline[i], t);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//berechnet die Daten der Spline für eine bestimmte Stelle
//@t: Position auf der Spline (zwischen 0 und 1);
//@next: Abstand zur nächsten Position;
function T3DSpline.GetSplineDataAt(t, next: Single): TSplineData;
var
  Delta, Point: TGLVector3f;
  d: Single;
begin
  result.Pos := GetSplineAt(t);
  Point      := GetSplineAt(t + Next);

  Delta[0] := result.Pos[0] - Point[0];
  Delta[1] := result.Pos[1] - Point[1];
  Delta[2] := result.Pos[2] - Point[2];

  d := SQRT(SQR(delta[0])+SQR(delta[1])+SQR(delta[2]));
  result.Angle[0] := ArcSin(delta[1]/d);
  result.Angle[1] := ArcTan2(delta[2],delta[0])-Pi/2;
  result.Angle[2] := 0;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
constructor T3DSpline.Create;
begin
  inherited Create;
  SetLength(fSpline[0], 0);
  SetLength(fSpline[1], 0);
  SetLength(fSpline[2], 0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei;
destructor T3DSpline.Destroy;
begin
  SetLength(fSpline[0], 0);
  SetLength(fSpline[1], 0);
  SetLength(fSpline[2], 0);
  inherited Destroy;
end;

end.

