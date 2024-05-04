void main()
{
    char* video_mem = (char*)0xb8000; // points to the first text cell of video memory (top left of screen)
    *video_mem = 'S'; // display this character at specified video memory address
}
