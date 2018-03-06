#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void wait (float time)
{
  clock_t koniec;
  koniec = clock () + time * CLOCKS_PER_SEC ;
  while (clock() < koniec) {}
}

int main(int argc, char *argv[])
{

  int scena = 1;
  char key = 0;
         
  static WINDOW *screen;
  screen = initscr();

  int res_x, res_y;
  getmaxyx(screen, res_y, res_x);    
  
  mvwprintw(screen, 0, 0, "Vyber scenu:");
  mvwprintw(screen, 1, 0, "~~~~~~~~~~~~");
  mvwprintw(screen, 3, 0, "1. Snehuliak");
  mvwprintw(screen, 4, 0, "2. Kyvadlo");
  wrefresh(screen);
  
  while (!(key == '1' || key == '2')) key = getchar();
  
  if (key == '2') scena = 2;
  else scena = 1;  

  cbreak();
  nodelay(screen, true);
         
  float pos_x = 0.0;
  float pos_y = 0.0;
  float pos_z = -20.0;
     
  float light_x = -5.0;
  float light_y = 5.0;
  float light_z = -11.0;
     
  float time = 0.0;
  char pixel;
  bool koniec = false;
     
  while (!koniec) {

    time = time + 0.05;
     
    float cos_y = cos(time);
    float sin_y = sin(time);
    
    // vypocitame poziciu kamery     
    pos_x = sin_y * 20.0;
    pos_y = sin_y * cos_y * 5;
    pos_z = cos_y * 20.0;
    
    float cos_y2 = cos(time * 8.0) * 6.0;
     
    int x,y;
     
    for (y = 0; y < res_y; y++){
     
      for (x = 0; x < res_x; x++){
        
        float targ_x, tar_x, tar_y, tar_z;
        
        targ_x = (1.0 - (x * 2.0) / res_x) * 13.0;
        tar_y = (1.0 - (y * 2.0) / res_y) * 7.5;
        //spocitame poziciu ciela
     
        tar_x = cos_y * targ_x;
        tar_z = -sin_y * targ_x;
        //pravouhle premietanie
     
        float ray_x = tar_x - pos_x;
        float ray_y = tar_y - pos_y;
        float ray_z = tar_z - pos_z;
        //spocitame vektor luca
     
        pixel = ' ';
        
        float z = 9999.0, sphere_x, sphere_y, sphere_z, sphere_radius;
        int n;
          
        for (n = 0; n < 7; n++){
                    
	  if (scena == 1)
          {
            switch(n)
            {
              case 0:          
                sphere_x = 0.0;
                sphere_y = -3.0;
                sphere_z = 1.0;
                sphere_radius = 4.5;
              break;
          
              case 1:
                sphere_x = 0.0;
                sphere_y = 3.0;
                sphere_z = 1.0;
                sphere_radius = 3.0;
              break;
              
              case 2:
                sphere_x = 0.0;
                sphere_y = 7.0;
                sphere_z = 1.0;
                sphere_radius = 2.0;
              break;
 
              case 3:
                sphere_x = 3.0;
                sphere_y = 3.5;
                sphere_z = 0.0;
                sphere_radius = 1.32;
              break;
            
              case 4:
                sphere_x = -3.0;
                sphere_y = 3.5;
                sphere_z = 0.0;
                sphere_radius = 1.32;
              break;
            
              case 5:
                sphere_x = 0.0;
                sphere_y = -10000.0;
                sphere_z = 0.0;
                sphere_radius = 9995;
              break;
            } //switch
          } 
          else
          if (scena == 2)
          {
            switch(n)
            {
              case 0:
                sphere_x = - 8.0 + cos_y2;
                sphere_y = 2.0 - cos_y2;
                sphere_z = 0.0;
                sphere_radius = 2;
             
                if (cos_y2 >= 0)
                {           
                  sphere_x = 0.0 - 8.0;
                  sphere_y = 2.0;
                }
              break;
           
             case 2:
                sphere_x = 0.0 - 4.0;
                sphere_y = 2.0;
                sphere_z = 0.0;
                sphere_radius = 2;
             break;
           
             case 3:
                sphere_x = 0.0;
                sphere_y = 2.0;
                sphere_z = 0.0;
                sphere_radius = 2;
             break;
             
             case 4:
                sphere_x = 4.0;
                sphere_y = 2.0;
                sphere_z = 0.0;
                sphere_radius = 2;
             break;
           
             case 5:
                sphere_x = 8.0 + cos_y2;
                sphere_y = 2.0 + cos_y2;
                sphere_z = 0.0;
                sphere_radius = 2;
             
               if (cos_y2 <= 0)
               {
                  sphere_x = 8.0;
                  sphere_y = 2.0;
               }
             break;
           
             case 6:
                sphere_x = 0.0;
                sphere_y = -10000.0;
                sphere_z = 0.0;
                sphere_radius = 9995;
             break;
          } // switch
        } // scena = 2	
           
          float sphere_radius2 = sphere_radius * sphere_radius;
          //spocita polomer gule na druhu
     
          float oc_x = pos_x - sphere_x;
          float oc_y = pos_y - sphere_y;
          float oc_z = pos_z - sphere_z;
          //spocitame vektor kamery do gule

          float a = ray_x * ray_x + ray_y * ray_y + ray_z * ray_z;
          float b = 2.0 * (ray_x * oc_x + ray_y * oc_y + ray_z * oc_z);
          float c = oc_x * oc_x + oc_y * oc_y + oc_z * oc_z - sphere_radius2;
          //riesime kvadraticku rovnicu
     
          float d = b * b - 4.0 * a * c;
          //spocitame diskriminant
           
          if (d > 0.0) {
            // ak je diskriminant kladny, mame priesecnik s gulou         
     
            float t = (-b - sqrt(d)) / (2.0 * a);
            // riesime kvadraticku rovnicu, ktorej vysledkom je
            // vzdialenost blizsieho prisecniku
     
            if ((t > 0) && (t <= z)){
                
                z = t;
                // z buffer pre kontrolu ktora gula je uplne vpredu
                // a teda ktora sa ma kreslit
                
                float intersection_x = ray_x * t + pos_x;
                float intersection_y = ray_y * t + pos_y;
                float intersection_z = ray_z * t + pos_z;
                // spocitame poziciu priesecnikov
         
                float normal_x = (intersection_x - sphere_x) / sphere_radius;
                float normal_y = (intersection_y - sphere_y) / sphere_radius;
                float normal_z = (intersection_z - sphere_z) / sphere_radius;
                // normala povrchu v bode priesecniku
         
                float to_light_x = light_x - intersection_x;
                float to_light_y = light_y - intersection_y;
                float to_light_z = light_z - intersection_z;
                // vektor do svetla
         
                d = sqrt(to_light_x * to_light_x + to_light_y * to_light_y + to_light_z * to_light_z);
                // spocitame vzdialenost na druhu

                float lightness = (normal_x * to_light_x + normal_y * to_light_y + normal_z * to_light_z) / d;
                // spocitame svetlost bodu ako dot
                // product vektoru lomeno vzdialenost
                
                if (lightness < 0.1) pixel = ' ';
                if ((lightness >= 0.1) && (lightness < 0.3)) pixel = '.';
                if ((lightness >= 0.3) && (lightness < 0.5)) pixel = '-';
                if ((lightness >= 0.5) && (lightness < 0.7)) pixel = '+';
                if ((lightness >= 0.7) && (lightness < 0.9)) pixel = '*';
                if (lightness > 0.9) pixel = '#';
            } // t > 0  &&  t <= z
          } //d <= 0
          //nemame priesecnik
        } //for n
     
        mvwprintw(screen, y, x, "%c", pixel);
        wrefresh(screen);
      } //for x
    } //for y
   
   wait(0.05);
   char vstup = getch();
   if (vstup == 27) koniec = true;
 /*  
   #ifdef __WIN32__
     system("cls");
   #else
     system("clear");   
   #endif
 */
    
  }
  
  endwin();
  return 0;
}
