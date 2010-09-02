unit uUtils;

{$mode objfpc}{$H+}

interface

uses
  dglOpenGL;

function glVector3f(X, Y, Z: Single): TglVector3f;
function glVector4f(X, Y, Z, W: Single): TVector4f;
procedure glRenderSkyBox;

implementation

//erzeugt einen Vektor aus den Parametern
function glVector3f(X, Y, Z: Single): TglVector3f;
begin
  result[0] := X;
  result[1] := Y;
  result[2] := Z;
end;

//erzeugt einen Vektor aus den Parametern
function glVector4f(X, Y, Z, W: Single): TVector4f;
begin
  result[0] := X;
  result[1] := Y;
  result[2] := Z;
  result[3] := W;
end;

//Zeichnet eine SkyBox
procedure glRenderSkyBox;
begin
  glBegin(GL_QUADS);
    //Z-
    glTexCoord2f(  0, 1/2); glVertex3f(-1, -1, -1);
    glTexCoord2f(1/3, 1/2); glVertex3f( 1, -1, -1);
    glTexCoord2f(1/3,   0); glVertex3f( 1,  1, -1);
    glTexCoord2f(  0,   0); glVertex3f(-1,  1, -1);
    //X-
    glTexCoord2f(2/3, 1/2); glVertex3f(-1, -1, -1);
    glTexCoord2f(2/3,   0); glVertex3f(-1,  1, -1);
    glTexCoord2f(1/3,   0); glVertex3f(-1,  1,  1);
    glTexCoord2f(1/3, 1/2); glVertex3f(-1, -1,  1);
    //Z+
    glTexCoord2f(1/3,   1); glVertex3f(-1, -1,  1);
    glTexCoord2f(1/3, 1/2); glVertex3f(-1,  1,  1);
    glTexCoord2f(  0, 1/2); glVertex3f( 1,  1,  1);
    glTexCoord2f(  0,   1); glVertex3f( 1, -1,  1);
    //X+
    glTexCoord2f(1/3,   1); glVertex3f( 1, -1, -1);
    glTexCoord2f(2/3,   1); glVertex3f( 1, -1,  1);
    glTexCoord2f(2/3, 1/2); glVertex3f( 1,  1,  1);
    glTexCoord2f(1/3, 1/2); glVertex3f( 1,  1, -1);
    //Y+
    glTexCoord2f(2/3, 1/2); glVertex3f(-1,  1, -1);
    glTexCoord2f(  1, 1/2); glVertex3f( 1,  1, -1);
    glTexCoord2f(  1,   0); glVertex3f( 1,  1,  1);
    glTexCoord2f(2/3,   0); glVertex3f(-1,  1,  1);
    //Y-
    glTexCoord2f(2/3, 1/2); glVertex3f(-1, -1, -1);
    glTexCoord2f(2/3,   1); glVertex3f(-1, -1,  1);
    glTexCoord2f(  1,   1); glVertex3f( 1, -1,  1);
    glTexCoord2f(  1, 1/2); glVertex3f( 1, -1, -1);
  glEnd;
  glClear(GL_DEPTH_BUFFER_BIT);
end;

end.

