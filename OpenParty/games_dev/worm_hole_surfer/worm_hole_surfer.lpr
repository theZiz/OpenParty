{
 The contents of this file are subject to the Mozilla Public License
 Version 1.1 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/

 Software distributed under the License is distributed on an "AS IS"
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 License for the specific language governing rights and limitations
 under the License.

 Alternatively, the contents of this file may be used under the terms
 of the GNU Lesser General Public license (the  "LGPL License"), in which case the
 provisions of LGPL License are applicable instead of those
 above.

 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
}
library worm_hole_surfer;
{$MODE OBJFPC}
{$H+}
{$IFDEF WINDOWS}
  {$CALLING cdecl}
{$ENDIF}
{$IFDEF UNIX}
  {$CALLING cdecl}
{$ENDIF}

uses
  SDL, SDL_Mixer, TTF, Minigames, SysUtils, Classes, Math,
  dglOpenGL,
  uWormHoleSurfer;

type
  //Variablen die das Spiel benutzt
  TGameData = packed record
	  GameInfo: PGameInfo;
    WormHoleSurfer: TWormHoleSurfer;
  end;
  PGameData = ^TGameData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//teilt dem Hostsystem mit, zu vielvielt und in welchen Temas das Spiel spielbar ist
//@GameInfo: Zeiger auf ein 2D-Array, welches die Kombinationen repräsentiert;
procedure op_get_gameinfos(GameInfo : PMiniGameInfo);
var
  i, j: Integer;
begin
  for i := 0 to 7 do
	  for j := 0 to 3 do
      if (j <> 2) and (i > 1) then
        GameInfo^.playable[i, j] := 1
      else
        GameInfo^.playable[i, j] := 0;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//teilt dem Hostsystem die größe des benötigten Speichers mit
//@result: Größe des benötigten Speichers in Byte;
function op_get_gamedata_size: LongInt;
begin
  result := SizeOf(TGameData);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//wird vom Hostsytem vor dem Spielstart aufgerufen und dient zur Initialisierung von Variablen
//@GameData: Zeiger auf dem vom Spiel reservierten Speicherbereich;
//@GameInfo: Zeiger auf ein Record mit den Daten zum Spiel
procedure op_init_game(GameData: PGameData; GameInfo: PGameInfo);
begin
	InitOpenGL;
	ReadExtensions;
  GameData^.GameInfo := GameInfo;
  GameData^.WormHoleSurfer := TWormHoleSurfer.Create(GameInfo);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//wird vom Hostsystem vor der RenderMethode innerhalb des Kontext des
//MainThreads aufgerufen und dient zur Berechnung von Variablen
//@GameData: Zeiger auf dem vom Spiel reservierten Speicherbereich;
//@result: wird ein Wert ungleich 0 zurückgegeben wird das Spiel beendet;
function op_calc_game(GameData: PGameData): LongInt;
begin
  with GameData^ do begin
    if GameInfo^.joystick[0].button[0]^ = 1 then
      exit(1);
    if WormHoleSurfer.Progress then
      exit(1);
  end;
  exit(0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//wird vom Hostsystem in einem extra Thread aufgerufen und dient zur Berechnung von Variablen
//@GameData: Zeiger auf dem vom Spiel reservierten Speicherbereich;
//@result: wird ein Wert ungleich 0 zurückgegeben wird das Spiel beendet;
function op_calc_game_thread(GameData : PGameData): LongInt;
begin
  if GameData^.GameInfo^.Shouldend^ > 0 then
	  exit(1);
	exit(0);
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//wird vom Hostsystem aufgerufen und dient zum Rendern der Daten
//@GameData: Zeiger auf dem vom Spiel reservierten Speicherbereich;
procedure op_draw_game(GameData: PGameData);
begin
  GameData^.WormHoleSurfer.Render;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//wird vom Hostsystem beim Beenden des Spiels aufgerufen und dient zur
//Freigabe von Objekten und Speicher sowie zur Spielergebnisauswertung
//@GameData: Zeiger auf dem vom Spiel reservierten Speicherbereich;
//@GameResult: Zeiger auf ein Array mit 8 Werten in denen gespeichert
//wird ob ein Spieler gewonnen hat oder nicht
procedure op_reset_game(GameData: PGameData; GameResult: PGameResult);
begin
  GameData^.WormHoleSurfer.SetGameResult(GameResult);
  GameData^.WormHoleSurfer.Free;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
end;

exports
  op_get_gameinfos,
  op_get_gamedata_size,
  op_init_game,
  op_calc_game,
  op_calc_game_thread,
  op_draw_game,
  op_reset_game;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,exOverflow, exUnderflow, exPrecision]);
end.
