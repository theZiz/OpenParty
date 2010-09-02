unit uParticles; 

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
  uSplineTube, uUtils,
  dglOpenGL;

type
  TParticleManager = class(TObject)
  private
    //Liste aller Partikel
    fParticleList: TList;
    //Wormloch-Objekt
    fSplineTube: TSplineTube;
  public
    procedure AddParticle(Pos, Roll, Speed, RollSpeed, LifeTime: Single);
    procedure Clear;
    procedure Render;
    procedure Progress(DeltaTime: Single);
    constructor Create(SplineTube: TSplineTube);
    destructor Destroy; override;
  end;

implementation

const
  FADE_OFF_TIME = 500;
  PARTICLE_HEIGHT = 0.8;

type
  TParticle = class(TObject)
    fLifeTime, fMaxLifeTime, fPos, fRoll, fTurn, fSpeed, fRollSpeed, fTurnSpeed: Single;
    fKill: Boolean;
    fColor: TVector4f;
    procedure Progress(DeltaTime: Single);
    procedure Render;
  end;

procedure TParticle.Progress(DeltaTime: Single);
begin
  fLifeTime := fLifeTime + DeltaTime;
  if fLifeTime > fMaxLifeTime + FADE_OFF_TIME then
    fKill := True;
  fPos  := fPos  + fSpeed     * DeltaTime/1000;
  fRoll := fRoll + fRollSpeed * DeltaTime/1000;
  fTurn := fTurn + fTurnSpeed * DeltaTime/1000;
end;

procedure TParticle.Render;
var Matrix: array[0..3, 0..3] of glFloat;
begin
  glgetFloatv(GL_MODELVIEW_MATRIX, @Matrix[0][0]);

  if (fLifeTime > fMaxLifeTime) then
    glColor4f(fColor[0], fColor[1], fColor[2],
      1-((fLifeTime-fMaxLifeTime)/FADE_OFF_TIME))
  else
    glColor4fv(@fColor[0]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glRotatef(fTurn, Matrix[0][2], Matrix[1][2], Matrix[2][2]);
  glBegin(GL_QUADS);
    glTexCoord2f(1,1); glVertex3f( Matrix[0][0]+Matrix[0][1],  Matrix[1][0]+Matrix[1][1],  Matrix[2][0]+Matrix[2][1]);
    glTexCoord2f(0,1); glVertex3f(-Matrix[0][0]+Matrix[0][1], -Matrix[1][0]+Matrix[1][1], -Matrix[2][0]+Matrix[2][1]);
    glTexCoord2f(0,0); glVertex3f(-Matrix[0][0]-Matrix[0][1], -Matrix[1][0]-Matrix[1][1], -Matrix[2][0]-Matrix[2][1]);
    glTexCoord2f(1,0); glVertex3f( Matrix[0][0]-Matrix[0][1],  Matrix[1][0]-Matrix[1][1],  Matrix[2][0]-Matrix[2][1]);
  glEnd;

  glDisable(GL_BLEND);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fügt ein Partikel hinzu
//@Pos: Position des Partikels in der SplineTube;
//@Roll: Drehwinkel des partikels in der SplineTube;
//@Speed: Geschwindigkeit der partikels ind er Tube;
//@RollSpeed: Drehgeschwindigkeit des partikels in der Tube;
//@LifeTime: Lebenszeit des Partikels (in ms);
procedure TParticleManager.AddParticle(Pos, Roll, Speed, RollSpeed, LifeTime: Single);
var NewPart: TParticle;
begin
  NewPart := TParticle.Create;
  NewPart.fLifeTime     := 0;
  NewPart.fMaxLifeTime  := LifeTime;
  NewPart.fTurn         := 0;
  NewPart.fTurnSpeed    := 360;
  NewPart.fPos          := Pos;
  NewPart.fSpeed        := Speed;
  NewPart.fRoll         := Roll;
  NewPart.fRollSpeed    := RollSpeed;
  NewPart.fKill         := False;
  NewPart.fColor        := glVector4f(1, 1, 1, 1);
  fParticleList.Add(NewPart);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//löscht alle Partikel
procedure TParticleManager.Clear;
begin
  while fParticleList.Count > 0 do begin
    TParticle(fParticleList[0]).Free;
    fParticleList.Delete(0);
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//zeichnet alle Partikel
procedure TParticleManager.Render;
var i: Integer;
begin
  for i := 0 to fParticleList.Count-1 do with TParticle(fParticleList[i]) do begin
    glPushMatrix;
      fSplineTube.SetOpenGLPos(fPos, fRoll, PARTICLE_HEIGHT);
      glScalef(0.5, 0.5, 0.5);
      TParticle(fParticleList[i]).Render;
    glPopMatrix;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//führt zeitabhängige Berechnungen durch
//@DeltaTime: Zeit (in ms) die seit dem letztem Renderdurchlauf vergangen ist;
procedure TParticleManager.Progress(DeltaTime: Single);
var i: Integer;
begin
  for i := fParticleList.Count-1 downto 0 do begin
    TParticle(fparticleList[i]).Progress(DeltaTime);
    if TParticle(fParticleList[i]).fKill then begin
      TParticle(fParticleList[i]).Free;
      fParticleList.Delete(i);
    end;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
//@SplineTube: Wurmloch in dem sich die Partikel befinden
constructor TParticleManager.Create(SplineTube: TSplineTube);
begin
  inherited Create;
  fSplineTube   := SplineTube;
  fParticleList := TList.Create;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei
destructor TParticleManager.Destroy;
begin
  Clear;
  fParticleList.Free;
  inherited Destroy;
end;

end.

