#include <iostream>
#include "raylib.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "game.h"

using namespace std;

const int grid_width = width/sprite_size;
const int grid_height = height/sprite_size;

int level_design[grid_height][grid_width];
int saved_level_design[grid_height][grid_width];

Texture2D background_texture;
Texture2D platform_texture;
Texture2D exit_texture;
Texture2D player_sprite_1_right_texture;
Texture2D player_sprite_2_right_texture;
Texture2D player_sprite_1_left_texture;
Texture2D player_sprite_2_left_texture;
Texture2D coin_texture;
Texture2D enemy_texture;

int blocks_left;
int saved_blocks_left;

int num_of_enemies = 0;
int saved_num_of_enemies;

struct coin{
  int coins_left;
  vector<Rectangle> coin_rects;
  int saved_coins_left;
  vector<Rectangle> saved_coin_rects;
  int dx_coin;
  int dy_coin;
};

struct enemy{
  vector<Rectangle> enemy_rects;
  vector<float> enemy_speed_x;
  vector<float> enemy_speed_y;
  vector<float> dx_enemy;
  vector<float> dy_enemy;
  vector<float> vy_enemy;
  vector<float> vx_enemy;
  vector<bool> isGrounded;
  vector<int> direction;
  vector<Rectangle> saved_enemy_rects;
  vector<float> saved_enemy_speed_x;
  vector<float> saved_enemy_speed_y;
  vector<float> saved_dx_enemy;
  vector<float> saved_dy_enemy;
  vector<float> saved_vy_enemy;
  vector<float> saved_vx_enemy;
  vector<bool> saved_isGrounded;
  vector<int> saved_direction;
};

struct player{
  int player_x;
  int player_y;
  int saved_player_x;
  int saved_player_y;
  Rectangle player_rect;
};

struct exit_portal{
  int exit_x;
  int exit_y;
  int saved_exit_x;
  int saved_exit_y;
  Rectangle exit_rect;
};

coin coins;
enemy enemies;
player main_player;
exit_portal exit_portal;

bool play;
bool build;

float gravity;
bool isGroundedPlayer;

int score = 0;
int saved_score;

double last_frame_switch;
double animation_speed;
int frame;

int play_button_x;
int play_button_y;
int play_button_width;
int play_button_height;

int build_button_x;
int build_button_y;
int build_button_width;
int build_button_height;

void clear_level(int level[grid_height][grid_width]){
  for(int i = 0; i < grid_height; i++){
    for(int j = 0; j < grid_width; j++){
      level[i][j] = 0;
    }
  }
}

void randomize_level(){
  blocks_left = rand() % 20 + 10;
  coins.coins_left = rand() % 10 + 1;
  num_of_enemies = rand() % 6;
  
  clear_level(level_design);
    
  exit_portal.exit_x = rand() % grid_width;
  exit_portal.exit_y = rand() % grid_height;
  exit_portal.exit_rect = {(float)exit_portal.exit_x * sprite_size, (float)exit_portal.exit_y * sprite_size, (float)sprite_size, (float)sprite_size};
  
  level_design[exit_portal.exit_y][exit_portal.exit_x] = 2;

  for(int i = 0; i < coins.coins_left; i++){
    coins.dx_coin = rand() % grid_width;
    coins.dy_coin = (rand() % (grid_height - 2)) + 1;
    coins.coin_rects.push_back({coins.dx_coin*sprite_size,coins.dy_coin*sprite_size,sprite_size,sprite_size});
    level_design[coins.dy_coin][coins.dx_coin] = 3;
  }

  for(int i = 0; i < num_of_enemies; i++){
    enemies.dx_enemy.push_back(rand() % grid_width);
    enemies.dy_enemy.push_back(rand() % grid_height);
    enemies.vx_enemy.push_back(rand() % 9 + 1);
    enemies.vy_enemy.push_back(rand() % 9 + 1);
    enemies.enemy_rects.push_back({enemies.dx_enemy[i]*sprite_size,enemies.dy_enemy[i]*sprite_size,sprite_size,sprite_size});
    enemies.enemy_speed_x.push_back(enemies.vx_enemy[i]);
    enemies.enemy_speed_y.push_back(enemies.vy_enemy[i]);
    enemies.isGrounded.push_back(false);
    enemies.direction.push_back((rand() % 2) * 2 - 1);
  }
  
  main_player.player_x = rand()%(width - sprite_size) + sprite_size;
  main_player.player_y = rand()%(height - sprite_size);

  main_player.saved_player_x = main_player.player_x;
  main_player.saved_player_y = main_player.player_y;
}

int init_level(){

  randomize_level();
  
  play = false;
  build = true;
  
  gravity = 2.0;
  
  last_frame_switch = GetTime();
  animation_speed = 0.25;
  frame = 0;

  play_button_x = 0.8*width;
  play_button_y = 0;
  play_button_width = 230;
  play_button_height = 100;

  build_button_x = 0.8*width;
  build_button_y = 0;
  build_button_width = 250;
  build_button_height = 100;
  
  isGroundedPlayer = false;
  return num_of_enemies;
}

void save_level(){
  for(int i = 0; i < grid_height; i++){
    for(int j = 0; j < grid_width; j++){
      saved_level_design[i][j] = level_design[i][j];
    }
  }

  exit_portal.saved_exit_x = exit_portal.exit_x;
  exit_portal.saved_exit_y = exit_portal.exit_y;
  
  main_player.saved_player_x = main_player.player_x;
  main_player.saved_player_y = main_player.player_y;

  coins.saved_coins_left = coins.coins_left;
  coins.saved_coin_rects = coins.coin_rects;

  enemies.saved_enemy_rects = enemies.enemy_rects;
  enemies.saved_direction = enemies.direction;
  enemies.saved_enemy_speed_x = enemies.enemy_speed_x;
  enemies.saved_enemy_speed_y = enemies.enemy_speed_y;
  enemies.saved_dx_enemy = enemies.dx_enemy;
  enemies.saved_dy_enemy = enemies.dy_enemy;
  enemies.saved_vx_enemy = enemies.vx_enemy;
  enemies.saved_vy_enemy = enemies.vy_enemy;
  enemies.saved_isGrounded = enemies.isGrounded;

  saved_blocks_left = blocks_left;
  saved_score = score;
  saved_num_of_enemies = num_of_enemies;
}

void restore_level(){
 for(int i = 0; i < grid_height; i++){
    for(int j = 0; j < grid_width; j++){
      level_design[i][j] = saved_level_design[i][j];
    }
}

 exit_portal.exit_x = exit_portal.saved_exit_x;
 exit_portal.exit_y = exit_portal.saved_exit_y;
 exit_portal.exit_rect = {(float)exit_portal.exit_x * sprite_size, (float)exit_portal.exit_y * sprite_size, (float)sprite_size, (float)sprite_size};
 
 main_player.player_x = main_player.saved_player_x;
  main_player.player_y = main_player.saved_player_y;
  
  coins.coin_rects = coins.saved_coin_rects;
  coins.coins_left = coins.coin_rects.size();

  for(int row = 0; row < grid_height; row++){
    for(int col = 0; col < grid_width; col++){
        if(level_design[row][col] == 3){
            level_design[row][col] = 0;
        }
    }
}
  
  for(int i = 0; i < coins.coin_rects.size(); i++){
    int grid_x = coins.coin_rects[i].x/sprite_size;
    int grid_y = coins.coin_rects[i].y/sprite_size;
    level_design[grid_y][grid_x] = 3;
  }
    
  enemies.enemy_rects = enemies.saved_enemy_rects;
  enemies.direction = enemies.saved_direction;
  enemies.enemy_speed_x = enemies.saved_enemy_speed_x;
  enemies.enemy_speed_y = enemies.saved_enemy_speed_y;
  enemies.dx_enemy = enemies.saved_dx_enemy;
  enemies.dy_enemy = enemies.saved_dy_enemy;
  enemies.vx_enemy = enemies.saved_vx_enemy;
  enemies.vy_enemy = enemies.saved_vy_enemy;
  enemies.isGrounded = enemies.saved_isGrounded;

  blocks_left = saved_blocks_left;
  score = saved_score;
  num_of_enemies = saved_num_of_enemies;
}

void draw_level(int level[grid_height][grid_width], int rows, int cols){// 1 = platform, 2 = exit, 3 = coins, 4 = enemies, 5 = player
	
for(int row = 0; row < rows; row++){
  for(int col = 0; col < cols; col++){
	  if(level[row][col] == 1){
	   DrawTexture(platform_texture,col*sprite_size,row*sprite_size,WHITE);
	  }

	  else if(level[row][col] == 2){
	    DrawTexture(exit_texture,col*sprite_size,row*sprite_size,WHITE);
	  }

	  else if(level[row][col] == 3){
	    DrawTexture(coin_texture,col*sprite_size,row*sprite_size,WHITE);
	  }
	  
	  else{
	    continue;
	  }
  }
 }
}

void run_game(){

  InitWindow(width,height,"Level Maker!");
  SetTargetFPS(60);
  InitAudioDevice();

  Sound get_coin = LoadSound("coin.mp3");
  Sound die = LoadSound("death.mp3");
  Sound next_level = LoadSound("exit.mp3");
  Sound place_item = LoadSound("place_item.mp3");
  Music main_music = LoadMusicStream("Game_music.mp3");
  
  Image background = LoadImage("Background.png");
  Image platform = LoadImage("platform.png");
  Image exit = LoadImage("exit.png");
  Image player_sprite_1_right = LoadImage("Character1_right.png");
  Image player_sprite_2_right = LoadImage("Character2_right.png");
  Image player_sprite_1_left = LoadImage("Character1_left.png");
  Image player_sprite_2_left = LoadImage("Character2_left.png");
  Image coin = LoadImage("coin.png");
  Image enemy = LoadImage("enemy.png");
  
  ImageResize(&background,width,height);
  ImageResize(&platform,sprite_size,sprite_size);
  ImageResize(&exit,sprite_size,sprite_size);
  ImageResize(&player_sprite_1_right,sprite_size,sprite_size);
  ImageResize(&player_sprite_2_right,sprite_size,sprite_size);
  ImageResize(&player_sprite_1_left,sprite_size,sprite_size);
  ImageResize(&player_sprite_2_left,sprite_size,sprite_size);
  ImageResize(&coin,sprite_size,sprite_size);
  ImageResize(&enemy,sprite_size,sprite_size);

  background_texture = LoadTextureFromImage(background);
  platform_texture = LoadTextureFromImage(platform);
  exit_texture = LoadTextureFromImage(exit);
  player_sprite_1_right_texture = LoadTextureFromImage(player_sprite_1_right);
  player_sprite_2_right_texture = LoadTextureFromImage(player_sprite_2_right);
  player_sprite_1_left_texture = LoadTextureFromImage(player_sprite_1_left);
  player_sprite_2_left_texture = LoadTextureFromImage(player_sprite_2_left);
  coin_texture = LoadTextureFromImage(coin);
  enemy_texture = LoadTextureFromImage(enemy);
  
  num_of_enemies = init_level();
  
  while(!WindowShouldClose()){

    BeginDrawing();
    DrawTexture(background_texture,0,0,WHITE);
    draw_level(level_design,grid_height,grid_width);

    DrawText(TextFormat("Blocks left to use: %i", blocks_left),0,0,55,RED);
    DrawText(TextFormat("Score: %i", score),650,0,55,GREEN);
    DrawText(TextFormat("Coins: %i",coins.coins_left),950,0,55,YELLOW); 
    DrawTexture(player_sprite_1_right_texture,main_player.player_x,main_player.player_y,WHITE);
      
    if(!play && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && blocks_left > 0 && level_design[GetMouseY()/sprite_size][GetMouseX()/sprite_size] == 0 && !(GetMouseX() >= main_player.player_x && GetMouseX() <= main_player.player_x + sprite_size && GetMouseY() >= main_player.player_y && GetMouseY() <= main_player.player_y + sprite_size)){
      level_design[GetMouseY()/sprite_size][GetMouseX()/sprite_size] = 1;
      blocks_left--;
      PlaySound(place_item);
    }

    if(!play and IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && GetMouseY() > play_button_y + play_button_height && level_design[GetMouseY()/sprite_size][GetMouseX()/sprite_size] == 1){
      level_design[GetMouseY()/sprite_size][GetMouseX()/sprite_size] = 0;
      blocks_left++;
      PlaySound(place_item);
    }
      
    if(!play && build){
      DrawRectangle(play_button_x,play_button_y,play_button_width,play_button_height,BLACK);
      DrawText("Play!",0.8*width,0,100,DARKBLUE);

      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMouseX() >= play_button_x && GetMouseX() <= play_button_x + play_button_width && GetMouseY() >= play_button_y && GetMouseY() <= play_button_y + play_button_height && !play && build){
	save_level();
	play = true;
	build = false;
      }
    }

    if(IsKeyPressed(KEY_R)){
      clear_level(level_design);
      init_level();
    }

    if(IsKeyPressed(KEY_Q)){
      CloseWindow();
      return;
    }
    
   if(play && !build){
      DrawRectangle(build_button_x,build_button_y,build_button_width,build_button_height,BLACK);
      DrawText("Build!",0.8*width,0,100,DARKBLUE);

      if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && GetMouseX() >= build_button_x && GetMouseX() <= build_button_x + build_button_width && GetMouseY() >= build_button_y && GetMouseY() <= build_button_y + build_button_height && play && !build){
       	restore_level();
	play = false;
	build = true;
      }

      PlayMusicStream(main_music);

      for(int i = 0; i < enemies.enemy_rects.size(); i++){   
      DrawTexture(enemy_texture,enemies.enemy_rects[i].x,enemies.enemy_rects[i].y,WHITE);
    }
	  
      if(GetTime() - last_frame_switch >= animation_speed){
      frame = (1 + frame) % 2;
      last_frame_switch = GetTime();
    }

      if(frame == 0 && IsKeyDown(KEY_RIGHT)){
      DrawTexture(player_sprite_1_right_texture,main_player.player_x,main_player.player_y,WHITE);
    }
    
      else if(frame == 1 && IsKeyDown(KEY_RIGHT)){
      DrawTexture(player_sprite_2_right_texture,main_player.player_x,main_player.player_y,WHITE);
    }

      else if(frame == 0 && IsKeyDown(KEY_LEFT)){
      DrawTexture(player_sprite_1_left_texture,main_player.player_x,main_player.player_y,WHITE);
    }
    
      else if(frame == 1 && IsKeyDown(KEY_LEFT)){
      DrawTexture(player_sprite_2_left_texture,main_player.player_x,main_player.player_y,WHITE);
    }
      else{
	DrawTexture(player_sprite_1_right_texture,main_player.player_x,main_player.player_y,WHITE);
      }

    if(IsKeyDown(KEY_RIGHT)){
      main_player.player_x += 5;
    }

    if(IsKeyDown(KEY_LEFT)){
      main_player.player_x -= 5;
    }

    main_player.player_y += gravity;

    int left_tile = main_player.player_x/sprite_size;
    int right_tile = (main_player.player_x + sprite_size - 1)/sprite_size;
    int feet_y = (main_player.player_y + sprite_size)/sprite_size;

    isGroundedPlayer = false;

    if(feet_y < grid_height){
      if(level_design[feet_y][left_tile] == 1 || level_design[feet_y][right_tile] == 1){
	main_player.player_y = (feet_y - 1) * sprite_size;
	isGroundedPlayer = true;
	gravity = 0;
    }
}

    if(!isGroundedPlayer){
      gravity = 2.0;
    }
    
    if(IsKeyDown(KEY_SPACE) && isGroundedPlayer){
      main_player.player_y -= 150;
      gravity = 2.0;
      isGroundedPlayer = false;
    }
    
    if(main_player.player_y >= height){
      PlaySound(die);
      restore_level();
    }

    if(main_player.player_x < 0){
      main_player.player_x = width - sprite_size;
    }

    if(main_player.player_x > width - sprite_size){
      main_player.player_x = 0;
    }

    for(int i = 0; i < enemies.enemy_rects.size(); i++){
      int enemy_grid_x = enemies.enemy_rects[i].x/sprite_size;
      int enemy_grid_y = enemies.enemy_rects[i].y/sprite_size;
      int left_tile = enemy_grid_x;
      int right_tile = (enemies.enemy_rects[i].x + sprite_size - 1)/sprite_size;
      int feet_y = (enemies.enemy_rects[i].y + sprite_size)/sprite_size;
      enemies.isGrounded[i] = false;

    if(feet_y < grid_height){
      if(level_design[feet_y][left_tile] == 1 || level_design[feet_y][right_tile] == 1){
	enemies.enemy_rects[i].y = (feet_y - 1) * sprite_size;
	enemies.isGrounded[i] = true;
    }
}

    if(enemies.isGrounded[i] == true){
    int next_tile = (enemies.direction[i] == -1) ? left_tile : right_tile;
    
    if(next_tile >= 0 && next_tile < grid_width && level_design[feet_y][next_tile] == 1){
    enemies.enemy_rects[i].x += enemies.direction[i];
    }

 else{
    enemies.direction[i] *= -1;
 }
    }
    
      if(enemies.isGrounded[i] == false){
      enemies.enemy_rects[i].y += 2;
      }

      if(enemies.enemy_rects[i].y >= height){
	enemies.enemy_rects[i].y = 0;
	enemies.enemy_rects[i].x = rand()%width;
    }
    }
    
    main_player.player_rect = {(float)main_player.player_x, (float)main_player.player_y, (float)sprite_size, (float)sprite_size};

    for(int i = 0; i < coins.coin_rects.size(); i++){
      if(CheckCollisionRecs(main_player.player_rect,coins.coin_rects[i])){
	coins.coins_left--;
	int coin_grid_x = coins.coin_rects[i].x/sprite_size;
	int coin_grid_y = coins.coin_rects[i].y/sprite_size;
	level_design[coin_grid_y][coin_grid_x] = 0;
	coins.coin_rects.erase(coins.coin_rects.begin() + i);
	PlaySound(get_coin);
      }
    }

    for(int i = 0; i < enemies.enemy_rects.size(); i++){
      if(CheckCollisionRecs(main_player.player_rect,enemies.enemy_rects[i])){
	PlaySound(die);
	restore_level();
      }
    }    
    
    if(CheckCollisionRecs(main_player.player_rect,exit_portal.exit_rect) && coins.coins_left == 0){
      PlaySound(next_level);
      clear_level(level_design);
      num_of_enemies = 0;
      init_level();
      score++;
    }
  }
    
  EndDrawing();
  }
  
  CloseWindow();
}


