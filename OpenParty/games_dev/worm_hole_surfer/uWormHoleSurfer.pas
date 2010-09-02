unit uWormHoleSurfer;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Minigames,
  uSplineTube, uUtils, uParticles,
  dglOpenGL, glVBOMesh, glLight;

type
  //Daten eines Spielers
  TPlayer = packed record
    //Nummer des Spielers in der HostAnwendung
    PlayerNum: Byte;
    //Farbe des Spielers
    Color: TVector4f;
    //Ist der Spieler ein BOT? (ja/nein)
    Bot: Boolean;
  end;

  //Daten eines Teams
  TTeam = packed record
    //Spieler im Team (0 = Steuermann; 1 = Schütze)
    Player: array[0..1] of TPlayer;
    //Nummer des Teams in der HostAnwendung
    TeamNum: Integer;
    //Position des Schiffs in der Röhre
    //|       Drehwinkel in der Röhre
    //|       |     Drehwinkel der Kanone
    //|       |     |           zählt die Zeit bis ein Schuss abgefeuert werden soll
    //|       |     |           |           Geschwindigkeit des Schiffs
    Position, Roll, CannonTurn, ShootCount, Speed: Single;
    //Sichtbereich des Teams
    Viewport: TVector4f;
    //Schuss aus dem Linken Lauf (ja/nein);
    ShootLeft: Boolean;
  end;

  //verwaltet alles was für das Spiel entscheident ist
  TWormHoleSurfer = class(TObject)
  private
    //Infos zum Spiel
    fGameInfo: PGameInfo;
    //Textur des Wurmlochs
    //|       Textur der SkyBox
    //|       |           Textur der Partikel
    fTubeTex, fSkyBoxTex, fParticleTex: gluInt;
    //das Wurmloch
    fSplineTube: TSplineTube;
    //die Teams
    fTeams: array of TTeam;
    //Model des Schiffs
    //|    Model der Kanone
    fShip, fCannon: TglVBOMesh;
    //LogFile des Games
    fLog: TStringList;
    //Viewport der HostAnwendung
    fOldViewport: TVector4i;
    //Licht
    fLight: TglLight;
    //Material der Schiffe
    fMaterial: TglMaterial;
    //verwaltet die Partikel
    fParticleManager: TParticleManager;
    procedure SetViewport(v: TVector4f);
  public
    procedure Render;
    function Progress: Boolean;
    procedure SetGameResult(GameResult: PGameResult);
    constructor Create(GameInfo: PGameInfo);
    destructor Destroy; override;
  end;

implementation

uses
  glMesh, Texture;

const
  CAMERA_HEIGHT       =      0.5;
  SHIP_HEIGHT         =      0.8;
  SHOOT_COUNT         =      200;
  PARTICLE_LIFE_TIME  =     1000;
  MIN_SPEED           = 0.000001;
  MAX_SPEED           = 0.000010;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRIVATE//PRI//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
procedure TWormHoleSurfer.SetViewport(v: TVector4f);
var
  Width, Height: Integer;
  Scale: Single;
begin
  Width  := fGameInfo^.windowx;
  Height := fGameInfo^.windowy;
  if (fGameInfo^.fullscreen = 1) then begin
    Width  := fGameInfo^.resolutionx;
    Height := fGameInfo^.resolutiony;
  end;
  Scale := (Width/Height)/(4/3);
  glViewport(
    Round(Width*v[0]),
    Round(Height*v[1]),
    Round(Width*v[2]),
    Round(Height*v[3]));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (fGameInfo^.stretched = 1) then
    gluPerspective(45, (Width*v[2])/(Scale*Height*v[3]), 0.001,1000)
  else
    gluPerspective(45, (Width*v[2])/(Height*v[3]), 0.001,1000);
  glMatrixMode(GL_MODELVIEW);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBLIC//PUBL//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//zeichnet das Minigame
procedure TWormHoleSurfer.Render;
var
  i, j: Integer;
begin
  glMatrixMode(GL_PROJECTION);
  glPushMatrix;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glLoadIdentity;

  for i := 0 to High(fTeams) do begin
    SetViewport(fTeams[i].Viewport);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity;

    //SkyBox zeichnen
    glColor4f(1,1,1,1);
    glPushMatrix;
      fSplineTube.SetOpenGLPos(fTeams[i].Position, fTeams[i].Roll, CAMERA_HEIGHT, True, True);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, fSkyBoxTex);
      glRenderSkyBox;
    glPopMatrix;

    //Camera setzen
    fSplineTube.SetOpenGLPos(fTeams[i].Position, fTeams[i].Roll, CAMERA_HEIGHT, True, False);

    //Schiffe zeichnen
    fLight.Bind(GL_LIGHT0);
    fMaterial.Bind;
    for j := 0 to High(fTeams) do begin
      glPushMatrix;
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        fSplineTube.SetOpenGLPos(fTeams[j].Position + 0.002, fTeams[j].Roll, SHIP_HEIGHT, False, False);
        glScalef(0.02, 0.02, 0.02);
        glColor4fv(@fTeams[j].Player[0].Color[0]);
        fShip.Render;
        glRotatef(fTeams[j].CannonTurn, 0, 1, 0);
        glColor4fv(@fTeams[j].Player[1].Color[0]);
        fCannon.Render;
      glPopMatrix;
    end;
    fLight.Unbind;

    //Tube zeichnen
    fSplineTube.Render;

    //Partikel zeichnen
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fParticleTex);
    fParticleManager.Render;
    glDisable(GL_TEXTURE_2D);
    glViewport(fOldViewport[0], fOldViewport[1], fOldViewport[2], fOldViewport[3]);
  end;

  glMatrixMode(GL_PROJECTION);
  glPopMatrix;
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//führt Berechnungen zum Game durch
//@result: TRUE, wenn das Spiel beendet werden soll, sonst FALSE;
function TWormHoleSurfer.Progress: Boolean;
var
  i: Integer;
  TmpSingle: Single;
begin
  for i := 0 to High(fTeams) do with fTeams[i] do begin
    if not Player[0].Bot then begin
      Roll     := Roll     - fGameInfo^.joystick[Player[0].PlayerNum].axis[0]^ * Word(fGameInfo^.steps^) * 0.1;
      TmpSingle := Speed;
      TmpSingle := TmpSingle - Word(fGameInfo^.steps^)*0.000000002;
      TmpSingle := TmpSingle - fGameInfo^.joystick[Player[0].PlayerNum].axis[1]^ * Word(fGameInfo^.steps^)*0.00000002;
      if (TmpSingle <> Speed) then begin
        Speed := TmpSingle;
        if Speed > MAX_SPEED then Speed := MAX_SPEED;
        if Speed < MIN_SPEED then Speed := MIN_SPEED;
      end;
    end;
    if not Player[1].Bot then begin
      TmpSingle := CannonTurn - fGameInfo^.joystick[Player[1].PlayerNum].axis[0]^ * Word(fGameInfo^.steps^) * 0.1;
      if (TmpSingle > -45) and (TmpSingle < 45) then
        CannonTurn := TmpSingle;
      if (fGameInfo^.joystick[Player[1].PlayerNum].button[0]^ = 1) then begin
        ShootCount := ShootCount + fGameInfo^.steps^;
        if (ShootCount >= SHOOT_COUNT) then begin
          ShootCount := ShootCount - SHOOT_COUNT;
          if ShootLeft then
            TmpSingle := Roll + 3
          else
            TmpSingle := Roll - 3;
          ShootLeft := not ShootLeft;
          fParticleManager.AddParticle(
            Position + 0.0015,
            TmpSingle,
            0.05*cos(CannonTurn/180*Pi),
            1000*sin(CannonTurn/180*Pi),
            PARTICLE_LIFE_TIME);
        end;
      end else if (ShootCount < SHOOT_COUNT-1) then
        ShootCount := SHOOT_COUNT-1;
    end;
    Position := Position + Word(fGameInfo^.steps^) * Speed;
  end;
  fParticleManager.Progress(fGameInfo^.steps^);
  result := false;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//legt das Ergebnis des Spiels fest
//@GameResult: Zeiger auf die Ergenise des Spiels;
procedure TWormHoleSurfer.SetGameResult(GameResult: PGameResult);
var i: Integer;
begin
  for i := 0 to fGameInfo^.playernum-1 do
    GameResult^.winner[i] := 1;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//erzeugt das Objekt
//@GameInfo: Zeiger auf die GameInfos des Spiels
constructor TWormHoleSurfer.Create(GameInfo: PGameInfo);
var
  Path: String;
  TmpInt, i: Integer;
  FileStream: TFileStream;
begin
  inherited Create;
  //Variablen initialisieren
  fLog      := TStringList.Create;
  fGameInfo := GameInfo;
  glGetIntegerv(GL_VIEWPORT, @fOldViewport[0]);

  //Texturen laden
  Path := String(GameInfo^.datafolder);
  if Path[Length(Path)] <> '/' then
    Path := Path + '/';
  Path := Path + 'textures/whs.clouds.png';
  fTubeTex := ZWloadtexturefromfileA(@path[1], 0, 1);

  Path := String(GameInfo^.datafolder);
  if Path[Length(Path)] <> '/' then
    Path := Path + '/';
  Path := Path + 'textures/whs.skybox.png';
  fSkyBoxTex := ZWloadtexturefromfile(@path[1], 0, 1);

  Path := String(GameInfo^.datafolder);
  if Path[Length(Path)] <> '/' then
    Path := Path + '/';
  Path := Path + 'textures/whs.particle.png';
  fParticleTex := ZWloadtexturefromfileA(@path[1], 0, 1);

  //Licht initialisieren
  fLight := TglLight.Create;
  fLight.SetAmbient(glVector4f(0.0, 0.0, 0.0, 1.0));
  fLight.SetDiffuse(glVector4f(0.7, 0.7, 0.7, 1.0));
  fLight.SetSpecular(glVector4f(0.5, 0.5, 0.5, 1.0));
  fLight.SetPosition(glVector4f(0.0, 0.0, 0.0, 1.0));

  //Material initialisieren
  fMaterial := TglMaterial.Create;
  fMaterial.SetSpecular(glVector4f(1.0, 1.0, 1.0, 1.0));
  fMaterial.SetDiffuse(glVector4f(0.9, 0.9, 0.9, 1.0));
  fMaterial.SetAmbient(glVector4f(0.8, 0.8, 0.8, 1.0));
  fMaterial.SetEmission(glVector4f(0.0, 0.0, 0.0, 1.0));
  fMaterial.Shininess := 25;

  //Tube initialisieren
  fSplineTube := TSplineTube.Create;
  fSplineTube.SetStartColor(glVector4f(0, 1, 0, 0.5));
  fSplineTube.SetEndColor(glVector4f(0, 0, 1, 0.5));
  fSplineTube.CreateRandom(10, 500, 500, 100);
  fSplineTube.Texture         := fTubeTex;
  fSplineTube.Circles         :=     1000;
  fSplineTube.PointsPerCircle :=       16;
  fSplineTube.TextureHeight   :=       10;
  fSplineTube.TubeSize        :=        2;
  fSplineTube.GenTube;

  fParticleManager := TParticleManager.Create(fSplineTube);

  //Models laden
  Path := String(fGameInfo^.datafolder);
  if Path[Length(Path)] <> '/' then
    Path := Path + '/';
  fShip := TglVBOMesh.Create;
  fShip.Mode := GL_TRIANGLES;
  i := 0;
  if FileExists(Path+'models/whs.ship.mdl') then begin
    FileStream := TFileStream.Create(Path+'models/whs.ship.mdl', fmOpenRead);
    FileStream.Read(i, SizeOf(Integer));
    fShip.VertexCount := i;
    FileStream.Read(  fShip.VertexData^, SizeOf(TVertexV3f)*i);
    FileStream.Read(fShip.TexCoordData^, SizeOf(TVertexT2f)*i);
    FileStream.Read(  fShip.NormalData^, SizeOf(TVertexN3f)*i);
    FileStream.Read(i, SizeOf(Integer));
    fShip.FaceCount := i div 3;
    FileStream.Read(fShip.FaceData^, SizeOf(gluInt)*i);
  end;
  fShip.UpdateVBO;
  fShip.VertexCount := 0;
  fShip.FaceCount   := 0;
  FileStream.Free;

  fCannon := TglVBOMesh.Create;
  fCannon.Mode := GL_TRIANGLES;
  if FileExists(Path+'models/whs.cannon.mdl') then begin
    FileStream := TFileStream.Create(Path+'models/whs.cannon.mdl', fmOpenRead);
    FileStream.Read(i, SizeOf(Integer));
    fCannon.VertexCount := i;
    FileStream.Read(  fCannon.VertexData^, SizeOf(TVertexV3f)*i);
    FileStream.Read(fCannon.TexCoordData^, SizeOf(TVertexT2f)*i);
    FileStream.Read(  fCannon.NormalData^, SizeOf(TVertexN3f)*i);
    FileStream.Read(i, SizeOf(Integer));
    fCannon.FaceCount := i div 3;
    FileStream.Read(fCannon.FaceData^, SizeOf(gluInt)*i);
  end;
  fCannon.UpdateVBO;
  fCannon.VertexCount := 0;
  fCannon.FaceCount   := 0;
  FileStream.Free;

  //Teams ermitteln
  TmpInt := fGameInfo^.playernum div fGameInfo^.teamsize;
  if (fGameInfo^.teamsize = 1) then begin
    TmpInt := TmpInt div 2;
    if (fGameInfo^.playernum mod 2 = 1) then
      inc(TmpInt);
  end;
  SetLength(fTeams, TmpInt);

  TmpInt := fGameInfo^.playernum;
  if (TmpInt mod 2 = 1) then
    inc(TmpInt);

  if (fGameInfo^.teamsize = 1) then begin
    for i := 0 to fGameInfo^.playernum-1 do begin
      fTeams[i div 2].Player[i mod 2].PlayerNum := i;
      fTeams[i div 2].Player[i mod 2].Bot       := (fGameInfo^.iscpu[i] = 1);
      fTeams[i div 2].Player[i mod 2].Color     :=
        glVector4f(
          fGameInfo^.color[i].r,
          fGameInfo^.color[i].g,
          fGameInfo^.color[i].b,
          fGameInfo^.color[i].a);
      fTeams[i div 2].TeamNum := (i div 2);
    end;
    if (fGameInfo^.playernum <> TmpInt) then begin
      fTeams[(TmpInt-1) div 2].Player[(TmpInt-1) mod 2].PlayerNum := TmpInt-1;
      fTeams[(TmpInt-1) div 2].Player[(TmpInt-1) mod 2].Bot       := True;
      fTeams[(TmpInt-1) div 2].Player[(TmpInt-1) mod 2].Color     := glVector4f(1, 1, 1, 1);
      fTeams[(TmpInt-1) div 2].TeamNum := (TmpInt-1) div 2;
    end;
  end;

  for i := 0 to High(fTeams) do begin
    fTeams[i].Position := 0;
    fTeams[i].CannonTurn := 0;
    fTeams[i].Roll := -(i-(Length(fTeams)/2)) * 25;
    fTeams[i].Speed := MIN_SPEED;
    if (Length(fTeams) = 2) then begin
      fTeams[i].Viewport := glVector4f(0.5*i, 0, 0.5, 1);
    end else begin
      if (i <= 1) then begin
        fTeams[i].Viewport := glVector4f(0.5*i, 0.5, 0.5, 0.5);
      end else begin
        if (Length(fTeams) = 3) then
          fTeams[i].Viewport := glVector4f(0, 0, 1, 0.5)
        else
          fTeams[i].Viewport := glVector4f(0.5*(i-2), 0, 0.5, 0.5);
      end;
    end;
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gibt das Objekt frei
destructor TWormHoleSurfer.Destroy;
begin
  fShip.Free;
  fCannon.Free;
  fSplineTube.Free;
  zwDeleteTexture(fTubeTex);
  zwDeleteTexture(fSkyBoxTex);
  zwDeleteTexture(fParticleTex);
  fLog.SaveToFile(fGameInfo^.datafolder+'/whs.logfile.log');
  fLog.Free;
  //glViewport(fOldViewport[0], fOldViewport[1], fOldViewport[2], fOldViewport[3]);
  inherited Destroy;
end;

end.

