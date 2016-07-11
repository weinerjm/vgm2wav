/* C example that opens a game music file and records 10 seconds to "out.wav" */
#include "gme/gme.h"

#include "wave_writer.h" /* wave_ functions for writing sound file */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#define T_SEC_DEFAULT 30

void handle_error( const char* str );

int main(int argc, char** argv)
{
    // Command-line arguments
    int vflag = 0;
    //char *infile = NULL;
    char infile[80];
    int iflag = 0;
    int c;
    int t_sec = 30; // default: 30 seconds of wave
    int tflag;
    int sel_voice;
    int sflag = 0;
    int oflag = 0;
    int verbose = 0;
    char *outfile = NULL;
    int trflag;
    int tr_sel;

    while ((c = getopt( argc, argv, "vbt:i:s:o:r:")) != -1 )
        switch (c)
        {
            case 'v': // all voices
              vflag = 1;
              break;
            case 'i': // input file
              strcpy(infile, optarg);
              iflag = 1;
              break;
            case 't': // change the time
              t_sec = atoi( optarg );
              tflag = 1;
              break;
            case 's': // single voice
              sel_voice = atoi( optarg );
              sflag = 1;
              break;
            case 'o':
              outfile = optarg;
              oflag = 1;
              break;
            case 'b':
              verbose = 1;
              break;
            case 'r':
              trflag = 1;
              tr_sel = atoi( optarg );
              break;
            case '?':
              if (optopt == 'i')
                  fprintf(stderr, "Option -%c requires an argument.\n", optopt);
              else if (isprint (optopt))
                  fprintf(stderr, "Unknown option `-%c'.\n", optopt);
              else
                  fprintf(stderr,
                          "Unknown option character `\\x%x'/\n",
                          optopt);
              return 1;
            default:
              abort();
        }
	
    long sample_rate = 44100; /* number of samples per second */
	int track = ( trflag == 1 ) ? tr_sel : 0; /* index of track to play (0 = first) */
	
    if (!iflag)
        sprintf(infile, "test.nsf");
    if (verbose)
        fprintf(stderr, "Input file : %s\n", infile);
	
    Music_Emu* emu;
	/* Open music file in new emulator */
	handle_error( gme_open_file( infile, &emu, sample_rate ) );
	
    /* Get num voices */
    int num_voices = ( vflag == 1 ) ? gme_voice_count( emu ) : 1;
    
    /* Decide on how long to play.
     * If time specified, play the whole track.
     * If no track length specified, default to 30 seconds.
     */
    if ( !tflag )
    {
        gme_info_t* tinfo;
        handle_error( gme_track_info( emu, &tinfo, track ) );
        int tlen = tinfo->play_length;
        if ( tinfo->length != -1 ) // if the length is specified
            t_sec = tinfo->length / 1000;
        else // if the length is not specified
            t_sec = (tlen == 150000) ? T_SEC_DEFAULT : (tlen / 1000);
        if (verbose)
            fprintf(stderr, "Track length : %d s\n", t_sec);
    }
    // process each voice if -v is enabled
    for (int vi = 0; vi < num_voices; vi++)
    {
        
        // unsilence tracks. 1 means mute
        #define ALL_VOICES -1
        if ( sflag == 1 ) 
        {
            gme_mute_voices( emu, ALL_VOICES );
            gme_mute_voice( emu, sel_voice, 0 ); 
            if (verbose)
                fprintf(stderr, "Unmuted voice %d\n", sel_voice);
        }
        else if ( vflag == 1 ) 
        {
            // unmute only the currently processed voice
            gme_mute_voices( emu, ALL_VOICES );
            gme_mute_voice( emu, vi, 0 );
            if (verbose)
                fprintf(stderr, "Unmuted voice %d\n", vi);
        }

        /* Start track */
        handle_error( gme_start_track( emu, track ) );
        
        char fname[80];
        if ( vflag == 1 )
            sprintf( fname, "Voice%d.wav", vi );
        else if ( oflag == 1 )
            strcpy( fname, outfile );
        else // no output set
        {
            if ( sflag == 1)
                sprintf( fname, "Voice%d.wav", sel_voice);
            else // default    
                sprintf( fname, "out.wav" );
        }

        if (verbose)
            fprintf(stderr, "Output file : %s\n", fname);
        
        /* Begin writing to wave file */
        FILE * curfile = wave_open( sample_rate, fname );
        wave_enable_stereo();
        
        /* Record t_sec (default 10) seconds of track */
        int done = 0;
        while ( gme_tell( emu ) < t_sec * 1000L )
        {
            /* Sample buffer */
            #define BUF_SIZE 1024 /* can be any multiple of 2 */
            short buf [BUF_SIZE];
            
            /* Fill sample buffer */
            handle_error( gme_play( emu, BUF_SIZE, buf ) );
            
            /* Write samples to wave file */
            wave_write( buf, BUF_SIZE );
        }

        /* Done writing voice to wave, write header */
        wave_write_header();
        /* Output temp file to stdout if -o - */ 
        if (strcmp (fname, "-" ) == 0)
        {
            rewind( curfile );
            size_t buflen;
            char buf[2048];
            while (( buflen = fread( buf, 1, sizeof buf, curfile )) > 0 )
                fwrite( buf, 1, buflen, stdout );
        }
	    /* now close the file */
        wave_close();
    }

	/* Cleanup */
	gme_delete( emu );
	
	return 0;
}

void handle_error( const char* str )
{
	if ( str )
	{
		printf( "Error: %s\n", str ); getchar();
		exit( EXIT_FAILURE );
	}
}
