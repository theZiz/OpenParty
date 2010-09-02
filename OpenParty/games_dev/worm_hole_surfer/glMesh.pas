unit glMesh;

{$mode objfpc}{$H+}

interface

uses
  DGLOpenGl,
  Classes, SysUtils;

type
  MeshException = Exception;

  TVertexV3f = packed record
    X, Y, Z: TglFloat;
  end;
  PVertexV3f = ^TVertexV3f;

  TVertexT2f = packed record
    S, T: TglFloat;
  end;
  PVertexT2f = ^TVertexT2f;

  TVertexN3f = packed record
    U, V, W: TglFloat;
  end;
  PVertexN3f = ^TVertexN3f;

  TglMesh = class(TObject)
  private
    //Name des Meshs
    fName: String;
    //MaterialIndex
    fMaterial: Integer;
    //Anzahl der Vertecs
    //|           Anzahl der Faces (Faces könenn Triangles oder Quads sein)
    fVertexCount, fFaceCount: Integer;
    //Set-Methoden
    procedure SetVertexCount(const Value: Integer);
    procedure SetFaceCount(const Value: Integer);
    procedure SetMode(const Value: Integer);
    procedure SetVertex(Index: Integer; const Value: TVertexV3f);
    procedure SetTexCoord(Index: Integer; const Value: TVertexT2f);
    procedure SetNormal(Index: Integer; const Value: TVertexN3f);
    procedure SetIndex(Index: Integer; const Value: gluInt);
    //Get-Methoden
    function GetVertex(Index: Integer): TVertexV3f;
    function GetTexCoord(Index: Integer): TVertexT2f;
    function GetNormal(Index: Integer): TVertexN3f;
    function GetIndex(Index: Integer): gluInt;

    function GetVertexPointer: Pointer;
    function GetTexCoordPointer: Pointer;
    function GetNormalPointer: Pointer;
    function GetFacePointer: Pointer;

  protected
    //Modus mit dem die VertexDaten gerendert werden sollen (GL_QUADS oder GL_TRIANGLES)
    fMode: Integer;
    //Vertexdaten (Position)
    fVertexArr: array of TVertexV3f;
    //Vertexdaten (Texturkoordinaten)
    fTexCoordArr: array of TVertexT2f;
    //Vertexdaten (Normalen)
    fNormalArr: array of TVertexN3f;
    //Indices der Vertecs
    fVertexIndex: array of gluInt;
  public
    property VertexCount: Integer read fVertexCount write SetVertexCount;
    property FaceCount  : Integer read fFaceCount   write SetFaceCount;
    property Mode       : Integer read fMode        write SetMode;

    property VertexData  : Pointer read GetVertexPointer;
    property TexCoordData: Pointer read GetTexCoordPointer;
    property NormalData  : Pointer read GetNormalPointer;
    property FaceData    : Pointer read GetFacePointer;

    property Vertex  [Index: Integer]: TVertexV3f read GetVertex   write SetVertex;
    property TexCoord[Index: Integer]: TVertexT2f read GetTexCoord write SetTexCoord;
    property Normal  [Index: Integer]: TVertexN3f read GetNormal   write SetNormal;
    property Indices [Index: Integer]: gluInt     read GetIndex    write SetIndex;

    constructor Create;
    destructor Destroy; override;
  end;


implementation

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//glMesh////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRI//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//legt die neue Anzahl an Vertecs fest
//@Value: neue Anzahl der Vertecs; 
procedure TglMesh.SetVertexCount(const Value: Integer);
begin
  if Value = fVertexCount then exit;
  fVertexCount := Value;
  SetLength(  fVertexArr, fVertexCount);
  SetLength(fTexCoordArr, fVertexCount);
  SetLength(  fNormalArr, fVertexCount);
end;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//legt die neue Anzahl an Faces fest
//@Value: neue Anzahl der Faces;
procedure TglMesh.SetFaceCount(const Value: Integer);
begin
  if fFaceCount = Value then exit;
  fFaceCount := Value;
  case fMode of
    GL_POINTS   : SetLength(fVertexIndex,     fFaceCount);
    GL_TRIANGLES: SetLength(fVertexIndex, 3 * fFaceCount);
    GL_QUADS    : SetLength(fVertexIndex, 4 * fFaceCount);
    else          SetLength(fVertexIndex, 0);
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//legt den Modus der Faces neu fest (GL_TRINAGLE oder GL_QUADS)
//ACHTUNG: alle alten Daten werden überschrieben, wenn der Modus neu gesetzt wird!
//@Value: neuer Modus der Faces;
procedure TglMesh.SetMode(const Value: Integer);
var i: Integer;
begin
  if fMode = Value then exit;
  fMode := Value;
  case fMode of
    GL_POINTS   : SetLength(fVertexIndex,     fFaceCount);
    GL_TRIANGLES: SetLength(fVertexIndex, 3 * fFaceCount);
    GL_QUADS    : SetLength(fVertexIndex, 4 * fFaceCount);
    else          SetLength(fVertexIndex, 0);
  end;
  for i := 0 to High(fVertexIndex) do
    fVertexIndex[i] := 0;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//schreibt ein Vertex in das Array
//@Index: Index an dem der Vertex abgelegt werden soll;
//@Value: Vertex der in das Array geschrieben werden soll;
procedure TglMesh.SetVertex(Index: Integer; const Value: TVertexV3f);
begin
  if (Index < 0) or (Index > High(fVertexArr)) then
    raise MeshException.Create('TglMesh.SetVertex: Index out of Bounds ('+IntToStr(Index)+')');
  fVertexArr[Index] := Value;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//schreibt eine TexturKoordinate in das Array
//@Index: Index an dem die TexturKoordinate abgelegt werden soll;
//@Value: TexturKoordinate die in das Array geschrieben werden soll;
procedure TglMesh.SetTexCoord(Index: Integer; const Value: TVertexT2f);
begin
  if (Index < 0) or (Index > High(fTexCoordArr)) then
    raise MeshException.Create('TglMesh.SetTexCoord: Index out of Bounds ('+IntToStr(Index)+')');
  fTexCoordArr[Index] := Value;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//schreibt eine Normale in das Array
//@Index: Index an dem die Normale abgelegt werden soll;
//@Value: Normale die in das Array geschrieben werden soll;
procedure TglMesh.SetNormal(Index: Integer; const Value: TVertexN3f);
begin
  if (Index < 0) or (Index > High(fNormalArr)) then
    raise MeshException.Create('TglMesh.SetNormal: Index out of Bounds ('+IntToStr(Index)+')');
  fNormalArr[Index] := Value;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//schreibt einen Index in das Array
//@Index: Index an dem der Index abgelegt werden soll;
//@Value: Index der in das Array geschrieben werden soll;
procedure TglMesh.SetIndex(Index: Integer; const Value: gluInt);
begin
  if (Index < 0) or (Index > High(fVertexIndex)) then
    raise MeshException.Create('TglMesh.SetIndex: Index out of Bounds ('+IntToStr(Index)+')');
  fVertexIndex[Index] := Value;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ließt einen Vertex aus dem Array
//@Index: Index des Vertex der gelesen werden soll;
//@result: Vertex aus dem Array;
function TglMesh.GetVertex(Index: Integer): TVertexV3f;
begin
  if (Index < 0) or (Index > High(fVertexArr)) then
    raise MeshException.Create('TglMesh.GetVertex: Index out of Bounds ('+IntToStr(Index)+')');
  result := fVertexArr[Index];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ließt eine TexturKoordinate aus dem Array
//@Index: Index der TexturKoordinate die gelesen werden soll;
//@result: TexturKoordinate aus dem Array;
function TglMesh.GetTexCoord(Index: Integer): TVertexT2f;
begin
  if (Index < 0) or (Index > High(fTexCoordArr)) then
    raise MeshException.Create('TglMesh.GetTexCoord: Index out of Bounds ('+IntToStr(Index)+')');
  result := fTexCoordArr[Index];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ließt eine Normale aus dem Array
//@Index: Index der Normalen die gelesen werden soll;
//@result: Normale aus dem Array;
function TglMesh.GetNormal(Index: Integer): TVertexN3f;
begin
  if (Index < 0) or (Index > High(fNormalArr)) then
    raise MeshException.Create('TglMesh.GetNormal: Index out of Bounds ('+IntToStr(Index)+')');
  result := fNormalArr[Index];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ließt einen Index aus dem Array
//@Index: Index des Index die gelesen werden soll;
//@result: Index aus dem Array;
function TglMesh.GetIndex(Index: Integer): gluInt;
begin
  if (Index < 0) or (Index > High(fVertexIndex)) then
    raise MeshException.Create('TglMesh.GetIndex: Index out of Bounds ('+IntToStr(Index)+')');
  result := fVertexIndex[Index];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt den Zeiger, hinter dem die Daten der Vertecs liegen
//@result: Zeiger auf die Vertecsdaten oder nil, wenn keine Vertecs vorhanden;
function TglMesh.GetVertexPointer: Pointer;
begin
  if fVertexCount = 0 then
    result := nil
  else
    result := @fVertexArr[0];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt den Zeiger, hinter dem die Daten der TexturKoordinaten liegen
//@result: Zeiger auf die Vertecsdaten oder nil, wenn keine TexturKoordinaten vorhanden;
function TglMesh.GetTexCoordPointer: Pointer;
begin
  if fVertexCount = 0 then
    result := nil
  else
    result := @fTexCoordArr[0];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt den Zeiger, hinter dem die Daten der Normalen liegen
//@result: Zeiger auf die Vertecsdaten oder nil, wenn keine Normalen vorhanden;
function TglMesh.GetNormalPointer: Pointer;
begin
  if fVertexCount = 0 then
    result := nil
  else
    result := @fNormalArr[0];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ermittelt den Zeiger, hinter dem die daten der Faces liegen
//@result: Zeiger auf die FaceDaten, oder nil;
function TglMesh.GetFacePointer: Pointer;
begin
  if fFaceCount = 0 then
    result := nil
  else
    result := @fVertexIndex[0];
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
constructor TglMesh.Create;
begin
  inherited Create;
  fName     := '';
  fMaterial := -1;
  fMode     := GL_TRIANGLES;
  SetVertexCount(0);
  SetFaceCount(0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei
destructor TglMesh.Destroy;
begin
  inherited Destroy;
end;

end.
