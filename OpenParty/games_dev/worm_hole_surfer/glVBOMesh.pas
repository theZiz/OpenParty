unit glVBOMesh;

{$mode objfpc}{$H+}

interface

uses
  DGLOpenGL,
  glMesh;

type
  TglVBOMesh = class(TglMesh)
  private
    //Vertex Buffer Objekt
    //|   GL_ELEMENT_ARRAY_BUFFER mit den Indices der Faces
    //|   |             Anzahl der Vertecs
    fVBO, fIndexBuffer, fVertCount: gluInt;
    //Offset der VertexDaten im VBO
    //|            Offset der TexturKoordinaten im VBO
    //|            |                Offset der Normalen im VBO
    fVertexOffset, fTexCoordOffset, fNormalOffset: Cardinal;
  public
    procedure UpdateVBO;
    procedure Render;
    constructor Create;
    destructor Destroy; override;
  end;

implementation

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//glVBOMesh/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRI//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//füllt das VBO mit den Daten des Meshs 
procedure TglVBOMesh.UpdateVBO;
var
  p: Pointer;
  i: Integer;
begin
  glBindBuffer(GL_ARRAY_BUFFER, fVBO);
  glBufferData(GL_ARRAY_BUFFER,
    Length(  fVertexArr) * SizeOf(TVertexV3f) +
    Length(fTexCoordArr) * SizeOf(TVertexT2f) +
    Length(  fNormalArr) * SizeOf(TVertexN3f),
    @fVertexArr[0], GL_STATIC_DRAW);
  p := glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  for i := 0 to High(fVertexArr) do begin
    PVertexV3f(p)^ := fVertexArr[i];
    inc(PVertexV3f(p));
  end;
  for i := 0 to High(fTexCoordArr) do begin
    PVertexT2f(p)^ := fTexCoordArr[i];
    inc(PVertexT2f(p));
  end;
  for i := 0 to High(fNormalArr) do begin
    PVertexN3f(p)^ := fNormalArr[i];
    inc(PVertexN3f(p));
  end;     
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    Length(fVertexIndex)*SizeOf(gluInt),
    @fVertexIndex[0],
    GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  fVertexOffset   := 0;
  fTexCoordOffset :=
    Length(  fVertexArr) * SizeOf(TVertexV3f);
  fNormalOffset   :=
    Length(  fVertexArr) * SizeOf(TVertexV3f) +
    Length(fTexCoordArr) * SizeOf(TVertexT2f);
  fVertCount := Length(fVertexIndex);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//zeichnet das Mesh
procedure TglVBOMesh.Render;
var
  Texture, Light: Boolean;
begin
  glGetBooleanv(GL_TEXTURE_2D, @Texture);
  glGetBooleanv(  GL_LIGHTING, @Light);

  glBindBuffer(GL_ARRAY_BUFFER, fVBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, Pointer(fVertexOffset));
  if Texture then begin
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, SizeOf(TVertexT2f), Pointer(fTexCoordOffset));
  end;
  if Light then begin
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, SizeOf(TVertexN3f), Pointer(fNormalOffset));
  end;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fIndexBuffer);

  glDrawElements(fMode, fVertCount, GL_UNSIGNED_INT, nil);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  if Texture then
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  if Light then
    glDisableClientState(GL_NORMAL_ARRAY); 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
constructor TglVBOMesh.Create;
begin
  inherited Create;

  fVertexOffset   := 0;
  fTexCoordOffset := 0;
  fNormalOffset   := 0;

  glGenBuffers(1, @fVBO);
  glGenBuffers(1, @fIndexBuffer);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei
destructor TglVBOMesh.Destroy;
begin
  glDeleteBuffers(1, @fVBO);
  glDeleteBuffers(1, @fINdexBuffer);
  inherited Destroy;
end;

end.
