#include "SDL.h"
#include "SDL_gpu.h"
#include <math.h>

#define IMAGE_FILE "data/test.bmp"
#define SAVE_FILE "save.bmp"

void printRenderers(void)
{
	const char* renderers[GPU_GetNumRegisteredRenderers()];
	GPU_GetRegisteredRendererList(renderers);
	
	printf("Available renderers:\n");
	int i;
	for(i = 0; i < GPU_GetNumRegisteredRenderers(); i++)
	{
		printf("%d) %s\n", i+1, renderers[i]);
	}
}

int main(int argc, char* argv[])
{
	printRenderers();
	
	GPU_Target* screen = GPU_Init(NULL, 800, 600, 0);
	if(screen == NULL)
    {
        GPU_LogError("Failed to init SDL_gpu.\n");
		return -1;
    }
	
	printf("Using renderer: %s\n", GPU_GetCurrentRendererID());
	
	GPU_LogError("Loading image\n");
	GPU_Image* image = GPU_LoadImage(IMAGE_FILE);
	if(image == NULL)
    {
        GPU_LogError("Failed to load image.\n");
		return -1;
    }
	
	GPU_LogError("Saving image\n");
	GPU_SaveImage(image, SAVE_FILE);
	
	GPU_LogError("Reloading image\n");
	GPU_Image* image1 = GPU_LoadImage(SAVE_FILE);
	if(image1 == NULL)
    {
        GPU_LogError("Failed to reload image.\n");
		return -1;
    }
    
	
	Uint32 startTime = SDL_GetTicks();
	long frameCount = 0;
	
	Uint8 done = 0;
	SDL_Event event;
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				done = 1;
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
			}
		}
		
		GPU_Clear(screen);
		
		GPU_Blit(image, NULL, screen, screen->w/4, screen->h/2);
		GPU_Blit(image1, NULL, screen, 3*screen->w/4, screen->h/2);
		
		GPU_Flip();
		
		frameCount++;
		if(frameCount%500 == 0)
			printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
	}
	
	printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
	
	GPU_FreeImage(image);
	GPU_FreeImage(image1);
	GPU_Quit();
	
	return 0;
}

