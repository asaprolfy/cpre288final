/*
 * sound.c
 *
 *  Created on: Apr 26, 2019
 *      Author: nlwolf
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <open_interface.h>
#include <inc/tm3c123gh6pm.h>
#include "Timer.h"
#include "Sound.h"


// \brief Load song sequence
// \param An integer value from 0 - 15 that acts as a label for note sequence
// \param An integer value from 1 - 16 indicating the number of notes in the sequence
// \param A pointer to a sequence of notes stored as integer values
// \param A pointer to a sequence of durations that correspond to the notes
//void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration)

// Plays a given song; use oi_load_song(...) first
//void oi_play_song(int index){
//    oi_uartSendChar(OI_OPCODE_PLAY);
//    oi_uartSendChar(index);
//}
/*struct song_info{

	int note[7];
	int duration[7];
};*/
//120 bpm => 2 bps => 32(48) for quarter note => 16(24) for double => 11(16),11(16),10(15) for triple;


void play_song(void){

	int i;
	int j;
	
	int note_value[10][7];

	note_value[0][0]=64; note_value[0][1]=57; note_value[0][2]=64;note_value[0][3]=0; note_value[0][4]=0; note_value[0][5]=0; note_value[0][6]=0;
	note_value[1][0]=69; note_value[1][1]=71; note_value[1][2]=73;note_value[1][3]=76; note_value[1][4]=0; note_value[1][5]=0; note_value[1][6]=0;
	note_value[2][0]=74; note_value[2][1]=73; note_value[2][2]=71;note_value[2][3]=73; note_value[2][4]=69; note_value[2][5]=0; note_value[2][6]=0;
	note_value[3][0]=71; note_value[3][1]=71; note_value[3][2]=76;note_value[3][3]=76; note_value[3][4]=74; note_value[3][5]=73; note_value[3][6]=71;
	note_value[4][0]=73; note_value[4][1]=69; note_value[4][2]=71;note_value[4][3]=71; note_value[4][4]=76; note_value[4][5]=0; note_value[4][6]=0;
	note_value[5][0]=76; note_value[5][1]=74; note_value[5][2]=73;note_value[5][3]=71; note_value[5][4]=73; note_value[5][5]=76; note_value[5][6]=0;
	note_value[6][0]=81; note_value[6][1]=81; note_value[6][2]=80;note_value[6][3]=78; note_value[6][4]=80; note_value[6][5]=76; note_value[6][6]=0;
	note_value[7][0]=78; note_value[7][1]=78; note_value[7][2]=76;note_value[7][3]=74; note_value[7][4]=76; note_value[7][5]=73; note_value[7][6]=0;
	note_value[8][0]=74; note_value[8][1]=74; note_value[8][2]=73;note_value[8][3]=71; note_value[8][4]=73; note_value[8][5]=69; note_value[8][6]=0;
	note_value[9][0]=65; note_value[9][1]=67; note_value[9][2]=69;note_value[9][3]=67; note_value[9][4]=0; note_value[9][5]=0; note_value[9][6]=0;

int note_duration[10][7];

	note_duration[0][0]=11; note_duration[0][1]=11; note_duration[0][2]=10; note_duration[0][3]=0; note_duration[0][4]=0; note_duration[0][5]=0; note_duration[0][6]=0;
	note_duration[1][0]=32; note_duration[1][1]=32; note_duration[1][2]=32; note_duration[1][3]=32; note_duration[1][4]=0; note_duration[1][5]=0; note_duration[1][6]=0;
	note_duration[2][0]=48; note_duration[2][1]=16; note_duration[2][2]=16; note_duration[2][3]=32; note_duration[2][4]=32; note_duration[2][5]=0; note_duration[2][6]=0;
	note_duration[3][0]=24; note_duration[3][1]=16; note_duration[3][2]=32; note_duration[3][3]=32; note_duration[3][4]=11; note_duration[3][5]=11; note_duration[3][6]=10;
	note_duration[4][0]=32; note_duration[4][1]=32; note_duration[4][2]=24; note_duration[4][3]=16; note_duration[4][4]=32; note_duration[4][5]=0; note_duration[4][6]=0;
	note_duration[5][0]=32; note_duration[5][1]=11; note_duration[5][2]=11; note_duration[5][3]=10; note_duration[5][4]=32; note_duration[5][5]=32; note_duration[5][6]=0;
	note_duration[6][0]=32; note_duration[6][1]=11; note_duration[6][2]=11; note_duration[6][3]=10; note_duration[6][4]=32; note_duration[6][5]=32; note_duration[6][6]=0;
	note_duration[7][0]=32; note_duration[7][1]=11; note_duration[7][2]=11; note_duration[7][3]=10; note_duration[7][4]=32; note_duration[7][5]=32; note_duration[7][6]=0;
	note_duration[8][0]=32; note_duration[8][1]=11; note_duration[8][2]=11; note_duration[8][3]=10; note_duration[8][4]=32; note_duration[8][5]=32; note_duration[8][6]=0;
	note_duration[9][0]=48; note_duration[9][1]=16; note_duration[9][2]=16; note_duration[9][3]=10; note_duration[9][4]=0; note_duration[9][5]=0; note_duration[9][6]=0;
	
	//struct song_info bar[10];

	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
		/*for(j=0;j<7;j++){
		bar[i].note[j] = note_value[i][j];
		bar[i].duration[j] = note_duration[i][j];
		}*/
		oi_loadSong(j,7,note_value[j+(i*4)],note_duration[j+(i*4)];
		oi_play_song(j);
		}
	}
}