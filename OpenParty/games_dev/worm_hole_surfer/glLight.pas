{$mode delphi}
unit glLight;

interface

uses
  DGLOpenGL, uUtils;

type
  TglMaterial = class(TObject)
  private
    fSpecular, fAmbient, fDiffuse, fEmission: TVector4f;
    fShininess: GLFloat;
  public
    property Shininess    : GLFloat   read fShininess   write fShininess;
    property Specular     : TVector4f read fSpecular    write fSpecular;
    property Ambient      : TVector4f read fAmbient     write fAmbient;
    property Diffuse      : TVector4f read fDiffuse     write fDiffuse;
    property Emission     : TVector4f read fEmission    write fEmission;
    procedure Bind;
    procedure SetSpecular(v: TVector4f);
    procedure SetAmbient(v: TVector4f);
    procedure SetDiffuse(v: TVector4f);
    procedure SetEmission(v: TVector4f);
    constructor Create; 
  end;
  
  TglLight = class(TObject)
  private
    fAmbient, fDiffuse, fSpecular, fPosition: TVector4f;
    fLight: Cardinal;
  public
    property Ambient      : TVector4f read fAmbient     write fAmbient;
    property Diffuse      : TVector4f read fDiffuse     write fDiffuse;
    property Position     : TVector4f read fPosition    write fPosition;
    procedure Bind(Light: Cardinal; EnableLight: Boolean = True);
    procedure Unbind(DisableLight: Boolean = True);
    procedure SetAmbient(v: TVector4f);
    procedure SetDiffuse(v: TVector4f);
    procedure SetSpecular(v: TVector4f);
    procedure SetPosition(v: TVector4f);
    constructor Create;
  end;

implementation

////////////////////////////////////////////////////////////////////////////////
//MATERIAL//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
procedure TglMaterial.Bind;
begin
  glMaterialfv(GL_FRONT, GL_SPECULAR,  @fSpecular[0]);
  glMaterialfv(GL_FRONT, GL_SHININESS, @fShininess);
  glMaterialfv(GL_FRONT, GL_AMBIENT,   @fAmbient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   @fDiffuse[0]);
  glMaterialfv(GL_FRONT, GL_EMISSION,  @fEmission[0]);
end;

procedure TglMaterial.SetSpecular(v: TVector4f);
begin
  fSpecular[0] := v[0];
  fSpecular[1] := v[1];
  fSpecular[2] := v[2];
  fSpecular[3] := v[3];
end;

procedure TglMaterial.SetAmbient(v: TVector4f);
begin
  fAmbient[0] := v[0];
  fAmbient[1] := v[1];
  fAmbient[2] := v[2];
  fAmbient[3] := v[3];
end;

procedure TglMaterial.SetDiffuse(v: TVector4f);
begin
  fDiffuse[0] := v[0];
  fDiffuse[1] := v[1];
  fDiffuse[2] := v[2];
  fDiffuse[3] := v[3];
end;

procedure TglMaterial.SetEmission(v: TVector4f);
begin
  fEmission[0] := v[0];
  fEmission[1] := v[1];
  fEmission[2] := v[2];
  fEmission[3] := v[3];
end;

constructor TglMaterial.Create;
begin
  inherited Create;
  fShininess := 0;
  SetSpecular(glVector4f(0.0, 0.0, 0.0, 0.0));
  SetAmbient (glVector4f(0.0, 0.0, 0.0, 0.0));
  SetDiffuse (glVector4f(0.0, 0.0, 0.0, 0.0));
  SetEmission(glVector4f(0.0, 0.0, 0.0, 0.0));
end;

////////////////////////////////////////////////////////////////////////////////
//LIGHT/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////[c]
procedure TglLight.Bind(Light: Cardinal; EnableLight: Boolean = True);
begin
  fLight := Light;
  glLightfv(Light, GL_AMBIENT,  @fAmbient[0]);
  glLightfv(Light, GL_DIFFUSE,  @fDiffuse[0]);
  glLightfv(Light, GL_POSITION, @fPosition[0]);
  glLightfv(Light, GL_SPECULAR, @fSpecular[0]);
  if EnableLight then glEnable(GL_LIGHTING);
  glEnable(fLight);
end;

procedure TglLight.Unbind(DisableLight: Boolean = True);
begin
  glDisable(fLight);
  if DisableLight then glDisable(GL_LIGHTING);
end;

procedure TglLight.SetAmbient(v: TVector4f);
begin
  fAmbient[0] := v[0];
  fAmbient[1] := v[1];
  fAmbient[2] := v[2];
  fAmbient[3] := v[3];
end;

procedure TglLight.SetDiffuse(v: TVector4f);
begin
  fDiffuse[0] := v[0];
  fDiffuse[1] := v[1];
  fDiffuse[2] := v[2];
  fDiffuse[3] := v[3];
end;

procedure TglLight.SetSpecular(v: TVector4f);
begin
  fSpecular[0] := v[0];
  fSpecular[1] := v[1];
  fSpecular[2] := v[2];
  fSpecular[3] := v[3];
end;

procedure TglLight.SetPosition(v: TVector4f);
begin
  fPosition[0] := v[0];
  fPosition[1] := v[1];
  fPosition[2] := v[2];
  fPosition[3] := v[3];
end;

constructor TglLight.Create;
begin
  inherited Create;
  SetAmbient (glVector4f(0.0, 0.0, 0.0, 0.0));
  SetDiffuse (glVector4f(0.0, 0.0, 0.0, 0.0));
  SetSpecular(glVector4f(0.0, 0.0, 0.0, 0.0));
  SetPosition(glVector4f(0.0, 0.0, 0.0, 0.0));
end;

end.
