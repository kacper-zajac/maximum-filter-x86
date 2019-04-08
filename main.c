#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_LOCKED_REGION *region = NULL;

extern long filtr(unsigned char *output_buffer, unsigned char *input_buffer, int size, int width, int height);

void init_display(int y, int x)
{
   if (display)
      al_destroy_display(display);

   display = al_create_display(y, x);

   if (!display)	// error
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }
}

void init()
{
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGB_888);
   if (!al_init()) // error
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   if (!al_init_image_addon()) // error
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }
}

void read_image_from_user()
{
   al_destroy_bitmap(image);
   image = NULL;
   char name[100];
   do
   {
      printf("Podaj nazwe pliku: ");
      scanf("%s", name);
      image = al_load_bitmap(name);
   } while (!image);
}

void end_program()
{
   al_destroy_display(display);
   al_destroy_bitmap(image);
   al_shutdown_image_addon();
}

int readSize()
{
	printf("Podaj wielkosc maski - ");
        int mask_size;
	scanf("%d", &mask_size);
	return mask_size;
}

int main(int argc, char *argv[])
{
   init();

   char control[10];
   while (true)
   {
     
      read_image_from_user();
      int width = al_get_bitmap_width(image);
      int height = al_get_bitmap_height(image);
	init_display(width, height-1);	
      int widthInBytes = width * 3;
      unsigned char *input_buffer = (unsigned char *)malloc(height * widthInBytes);

      region = al_lock_bitmap(image,ALLEGRO_PIXEL_FORMAT_RGB_888, ALLEGRO_LOCK_READWRITE);
      memcpy(input_buffer, ((unsigned char *)(region->data)) - (height - 1) * widthInBytes, height * widthInBytes);
      al_unlock_bitmap(image);

      while (true)
      {
         int mask_size = readSize();
         region = al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_RGB_888, ALLEGRO_LOCK_READWRITE);

         filtr((unsigned char *)(region->data) + widthInBytes - 1, input_buffer + (height - 1) * widthInBytes - 1, mask_size, width, height);
         al_unlock_bitmap(image);

         al_draw_bitmap(image, 0, 0, 0);
         al_flip_display();

         printf("Chcesz uzyc innego rozmiaru maski?(y/n) ");
         scanf("%s", control);
         if (control[0] != 'y')
            break;
      }

      printf("Chcesz uzyc innego pliku?(y/n) ");
      scanf("%s", control);
      if (control[0] != 'y')
      {
         free(input_buffer);
         break;
      }
   }

   end_program();

   return 0;
}
