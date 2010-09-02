unit uSplineTube;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
  u3DSpline, uUtils,
  DGLOpenGL;

type
  TSplineTube = class(T3DSpline)
  private
    //Anzahl der Kreise
    //|       Anzahl der Punkte pro Kreis
    fCircles, fPointsPerCircle: Integer;
    //Radius der Röhre
    //|        Anzahl der TexturWiderholungen in der Röhre
    //|        |          Anzahl der Texturwiederholungen zwischen 2 Stützpunkten
    //|        |          |           Drehung der Textur entlang der Röhre
    fTubeSize, fTexWidth, fTexHeight, fTexRoll: Single;
    //Farbe am Anfang der Röhre
    //|          Farbe am Ende der Röhre
    fStartColor, fEndColor: TVector4f;
    //ID des VertexBuffer Objekts
    fVBO: gluInt;
    //Indices der Vertecs
    fVertexIndices: array of gluInt;
    //Zeiger auf die Daten im VertexIndicesArray
    fQuadIndices: array of ^glVoid;
    //Anzahl der zu zeichnenden Vertex für die Primitiven
    fIndicesCount: array of glSizei;
    //Textur der Röhre
    fTexture: gluInt;
    procedure GenVertexData;
    procedure GenVertexIndices;
  public
    property Circles        : Integer   read fCircles         write fCircles;
    property PointsPerCircle: Integer   read fPointsPerCircle write fPointsPerCircle;
    property TubeSize       : Single    read fTubeSize        write fTubeSize;
    property TextureWidth   : Single    read fTexWidth        write fTexWidth;
    property TextureHeight  : Single    read fTexHeight       write fTexHeight;
    property TextureRoll    : Single    read fTexRoll         write fTexRoll;
    property Texture        : gluInt    read fTexture         write fTexture;
    property StartColor     : TVector4f read fStartColor;
    property EndColor       : TVector4f read fEndColor;

    procedure SetStartColor(v: TVector4f);
    procedure SetEndColor(v: TVector4f);

    function GetTubePos(t, Roll: Single): TglVector3f;
    procedure GenTube;
    procedure SetOpenGLPos(Pos, Roll, Height: Single; AsCam: Boolean = False; RotOnly: Boolean = false);
    procedure Render;
    procedure CreateRandom(n: Integer; x, y, z: Single);
    constructor Create;
    destructor Destroy; override;
  end;

implementation

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TSplineTube///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRI//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//legt die VertexDatan im VBO an
procedure TSplineTube.GenVertexData;
type
  TVertex = packed record
    X, Y, Z,
    R, G, B, A,
    S, T: TglFloat;
  end;
  PVertex = ^TVertex;
var
  //Anzahl der Vertecs im VBO
  //|          Schleifenvariable
  //|          |  Schleifenvariable
  VertexCount, i, j: Integer;
  //Adresse des aktuellen Vertecs
  Adress: PVertex;

  //schreibt die Position in das Vertex
  //@Pos: Position auf der Spline (zwischen 0 und 1);
  //@Roll: Drehwingel in der Tube (im Bodenmaß);
  procedure SetVertexPos(Pos, Roll: Single);
  var v: TglVector3f;
  begin
    v := GetTubePos(Pos, Roll);
    Adress^.X := v[0];
    Adress^.Y := v[1];
    Adress^.Z := v[2];
  end;
  //schreibt die Farbe in das Vertex
  //@Pos: Position auf der Spline (zwischen 0 und 1);
  procedure SetVertexColor(Pos: Single);
  begin
    Adress^.R := (1-Pos)*fStartColor[0] + Pos*fEndColor[0];
    Adress^.G := (1-Pos)*fStartColor[1] + Pos*fEndColor[1];
    Adress^.B := (1-Pos)*fStartColor[2] + Pos*fEndColor[2];
    Adress^.A := (1-Pos)*fStartColor[3] + Pos*fEndColor[3];
  end;
  //schreibt die TexturKoordinaten in das Vertex
  //@Pos: Position auf der Spline (zwischen 0 und 1);
  //@Roll: Drehwingel in der Tube (zwischen 0 und 1);
  procedure SetVertexTexCoord(Pos, Roll: Single);
  begin
    Adress^.S :=  Roll * fTexWidth + Pos * fTexRoll;
    Adress^.T := -Pos  * fTexHeight * Count;
  end;
begin
  glBindBufferARB(GL_ARRAY_BUFFER, fVBO);
  VertexCount := (fPointsPerCircle+1)*(fCircles);
  glBufferDataARB(GL_ARRAY_BUFFER, VertexCount * SizeOf(TVertex), nil, GL_STATIC_DRAW);
  Adress := glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

  for i := fCircles-1 downto 0 do begin
    for j := fPointsPerCircle downto 0 do begin
      SetVertexColor(i/fCircles);
      SetVertexTexCoord(i/fCircles, j/fPointsPerCircle);
      SetVertexPos(i/fCircles, 2*Pi*j/fPointsPerCircle);
      inc(Adress);
    end;
  end;

  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//berechnet die Indices für die Vertecs neu
procedure TSplineTube.GenVertexIndices;
  //berechnet den Index des Vertexs im VBO
  //@Circle: aktueller KreisIndex auf der Tube;
  //@Point: aktueller PunktIndex auf dem Kreis;
  function GetVBOIndex(Circle, Point: Integer): Integer;
  begin
    while Circle >       fCircles-1 do Circle := Circle - fCircles;
    while Circle <                0 do Circle := Circle + fCircles;
    while Point  > fPointsPerCircle do Point  := Point  - fPointsPerCircle - 1;
    while Point  <                0 do Point  := Point  + fPointsPerCircle + 1;
    result := (fPointsPerCircle+1)*(Circle)+Point;
  end;
var
//Schleifenvariablen
//|  |  benötigte Länge für das IndexArray
//|  |  |    Zählvariable
  i, j, len, c: Integer;
begin
  len := (fCircles-1)*(fPointsPerCircle+1)*2;
  if Length(fVertexIndices) <> len then begin
    SetLength(fVertexIndices,        len);
    SetLength(  fQuadIndices, fCircles-1);
    SetLength( fIndicesCount, fCircles-1);
  end;

  c := 0;
  for i := 0 to fCircles-2 do begin
    fQuadIndices[i]  := @fVertexIndices[c];
    fIndicesCount[i] := (fPointsPerCircle+1)*2;
    for j := 0 to fPointsPerCircle do begin
      fVertexIndices[c] := GetVBOIndex(i+1,j); inc(c);
      fVertexIndices[c] := GetVBOIndex(i  ,j); inc(c);
    end;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Leg die StartFarbe neu fest
//@v: neue Startfarbe;
procedure TSplineTube.SetStartColor(v: TVector4f);
begin
  fStartColor[0] := v[0];
  fStartColor[1] := v[1];
  fStartColor[2] := v[2];
  fStartColor[3] := v[3];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Leg die EndFarbe neu fest
//@v: neue EndFarbe;
procedure TSplineTube.SetEndColor(v: TVector4f);
begin
  fEndColor[0] := v[0];
  fEndColor[1] := v[1];
  fEndColor[2] := v[2];
  fEndColor[3] := v[3];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt einen Punkt in der Röhre
function TSplineTube.GetTubePos(t, Roll: Single): TglVector3f;
var
  d, e: Single;
  SplineData: TSplineData;
begin
  result[0] := 0;
  result[1] := 0;
  result[2] := 0;

  SplineData := GetSplineDataAt(t, 1/fCircles);
  //Kreis erzeugen
  result[0] := cos(Roll)*fTubeSize;
  result[1] := sin(Roll)*fTubeSize;
  result[2] := 0;
  //Drehung um die Y-Achse
  d := result[0];
  result[0] := cos(SplineData.Angle[1]) * d;
  result[2] := sin(SplineData.Angle[1]) * d;
  //Drehung um die X-Achse
  d := result[1];
  result[1] := d *  cos(SplineData.Angle[0]);
          e := d * -sin(SplineData.Angle[0]);
  result[0] := result[0] - e * sin(SplineData.Angle[1]);
  result[2] := result[2] + e * cos(SplineData.Angle[1]);
  //fertig gedrehten Kreis an die richtige Stelle verschieben
  result[0] := result[0] + SplineData.Pos[0];
  result[1] := result[1] + SplineData.Pos[1];
  result[2] := result[2] + SplineData.Pos[2];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt die Tube aus den SplineDaten
procedure TSplineTube.GenTube;
begin
  GenSpline;
  GenVertexData;
  GenVertexIndices;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Manipuliert die OpenGL Matricen so, das der Zeichenstift auf der gewünschten Stelle der Röhre steht
//@Pos: Position auf der Spline (zwischen 0 und 1);
//@Roll: Drehwinkel in der Röhre (im Gradmaß);
//@Height: Höhe in der Röhre (0 = Mittelpunkt, 1 = Boden);
//@AsCam: So manipulieren, das die Kamera an die Stelle versetz wird;
//@RotOnly: Nur die Drehbewegungen ausführen (z.B. für Skybox nützlich);
procedure TSplineTube.SetOpenGLPos(Pos, Roll, Height: Single; AsCam: Boolean = False; RotOnly: Boolean = false);
var SplineData: TSplineData;
begin
  SplineData := GetSplineDataAt(Pos, 1/fCircles);

  if not AsCam then begin
    if not RotOnly then
      glTranslatef(SplineData.Pos[0],
                   SplineData.Pos[1],
                   SplineData.Pos[2]);
    glRotatef(-SplineData.Angle[1]/Pi*180,0,1,0);
    glRotatef(-SplineData.Angle[0]/Pi*180,1,0,0);
    glRotatef(-Roll,0,0,1);
    if not RotOnly then
      glTranslatef(0,-fTubeSize*Height,0);
  end else begin
    if not RotOnly then
      glTranslatef(0,fTubeSize*Height,0);
    glRotatef(Roll,0,0,1);
    glRotatef(SplineData.Angle[0]/Pi*180,1,0,0);
    glRotatef(SplineData.Angle[1]/Pi*180,0,1,0);
    if not RotOnly then
      glTranslatef(-SplineData.Pos[0],
                   -SplineData.Pos[1],
                   -SplineData.Pos[2]);
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//zeichnet die Tube
procedure TSplineTube.Render;
const
  OFFSET1 = 12;
  OFFSET2 = 28;
  STRIDE  = 36;
var
  TexturesEnabled: Boolean;
begin
  if fTexture <> 0 then begin
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fTexture);
  end else glDisable(GL_TEXTURE_2D);

  glGetBooleanv(GL_TEXTURE_2D, @TexturesEnabled);
  glDepthMask(False);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glBindBufferARB(GL_ARRAY_BUFFER, fVBO);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, STRIDE, nil);

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, STRIDE, Pointer(OFFSET1));

  if TexturesEnabled then begin
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, STRIDE, Pointer(OFFSET2));
  end;

  glMultiDrawElements(GL_QUAD_STRIP,
    @fIndicesCount[0],
    GL_UNSIGNED_INT,
    @fQuadIndices[0],
    Length(fIndicesCount));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  if TexturesEnabled then begin
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  end;

  glBindBufferARB(GL_ARRAY_BUFFER, 0);

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glDepthMask(True);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt eine Röhre aus zufälligen Punkten
//@Points: Anzahl der Stützpunkte
//@x: Bereich auf der4 X-Achse (-x/2 bis x/2);
//@y: Bereich auf der Y-Achse (-y/2 bis y/2);;
//@z: konstante Differenz auf der Z-Achse;
procedure TSplineTube.CreateRandom(n: Integer; x, y, z: Single);
var
  i: Integer;
  p: TglVector3f;
begin
  randomize;
  Clear;
  p := glVector3f(0, 0, 0);
  Add(p);
  for i := 0 to n-1 do begin
    p[0] := p[0] + random(Round(x*1000))/1000 - x/2;
    p[1] := p[1] + random(Round(y*1000))/1000 - y/2;
    p[2] := p[2] + z;
    Add(p);
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
constructor TSplineTube.Create;
begin
  inherited Create;
  glGenBuffers(1, @fVBO);
  fTubeSize  := 10;
  fTexWidth  :=  1;
  fTexHeight :=  3;
  fTexRoll   :=  0;

  fStartColor[0] := 0;
  fStartColor[1] := 1;
  fStartColor[2] := 0;
  fStartColor[3] := 1;

  fEndColor[0] := 0;
  fEndColor[1] := 0;
  fEndColor[2] := 1;
  fEndColor[3] := 1;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei
destructor TSplineTube.Destroy;
begin
  glDeleteBuffers(1, @fVBO);
  inherited Destroy;
end;

end.

