
;;; source file for CC65 "remake" compiler
	.include "soundbs.mac"
	.global musicptr
	.rodata
musicptr:
	.word CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3

PATTERN_LENGTH   .set  1280

CHANNEL_0:
	PATTERN ENVIROS
	DO 0
	PATTERN PATTERN_01
	PATTERN PATTERN_01
	PATTERN PATTERN_01
	PATTERN PATTERN_01
	PATTERN PATTERN_02
	PATTERN PATTERN_02
	LOOP
;	DELAY PATTERN_LENGTH
CHAN_STOP

CHANNEL_1:
	PATTERN ENVIROS
	DO 0
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	PATTERN PATTERN_04
	PATTERN PATTERN_04
	PATTERN PATTERN_05
	PATTERN PATTERN_05
	LOOP
;	DELAY PATTERN_LENGTH
CHAN_STOP

CHANNEL_2:
	PATTERN ENVIROS
	DO 0
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	PATTERN PATTERN_00
	LOOP
;	DELAY PATTERN_LENGTH
CHAN_STOP

CHANNEL_3:
	PATTERN ENVIROS
	DO 0
	PATTERN PATTERN_03
	PATTERN PATTERN_03
	PATTERN PATTERN_03
	PATTERN PATTERN_03
	PATTERN PATTERN_03
	PATTERN PATTERN_03
	LOOP
;	DELAY PATTERN_LENGTH
CHAN_STOP


ENVIROS:
	SETPLAYERFREQ $9D,129
	DEF_VOLENV 1,TREMOLO_1
	DEF_VOLENV 2,TREMOLO_2
	DEF_VOLENV 3,TREMOLO_3
	DEF_VOLENV 4,TREMOLO_4
	DELAY 3
RETURN

TREMOLO_1:
.byte $01,$02,$02,$00,$03,$FF
TREMOLO_2:
.byte $02,$05,$07,$04,$05,$FC,$05,$04,$05,$FF,$01,$F8
TREMOLO_3:
.byte $02,$02,$01,$00,$01,$FB
TREMOLO_4:
.byte $02,$02,$0B,$00,$01,$FE



PATTERN_00:
	DELAY 1280
RETURN


PATTERN_01:
	INSTR2 $00,$80,$84,75,127
	SET_VOLENV 1
	PLAY2 132,$79,80
	PLAY2 132,$79,40
	PLAY2 132,$79,40
	PLAY2 132,$79,80
	PLAY2 132,$79,80
	PLAY2 132,$79,80
	PLAY2 132,$79,40
	PLAY2 132,$79,40
	PLAY2 132,$79,80
	PLAY2 132,$79,80
	PLAY2 74,$7a,80
	PLAY2 74,$7a,40
	PLAY2 74,$7a,40
	PLAY2 74,$7a,80
	PLAY2 74,$7a,80
	PLAY2 118,$79,80
	PLAY2 118,$79,40
	PLAY2 118,$79,40
	PLAY2 118,$79,80
	PLAY2 118,$79,80
RETURN


PATTERN_02:
	INSTR2 $00,$80,$84,75,127
	SET_VOLENV 1
	PLAY2 74,$7a,80
	PLAY2 74,$7a,40
	PLAY2 74,$7a,40
	PLAY2 74,$7a,80
	PLAY2 74,$7a,80
	PLAY2 118,$79,80
	PLAY2 118,$79,40
	PLAY2 118,$79,40
	PLAY2 118,$79,80
	PLAY2 118,$79,80
	PLAY2 132,$79,80
	PLAY2 132,$79,40
	PLAY2 132,$79,40
	PLAY2 132,$79,80
	PLAY2 132,$79,80
	PLAY2 132,$79,80
	PLAY2 132,$79,40
	PLAY2 132,$79,40
	PLAY2 132,$79,80
	PLAY2 132,$79,80
RETURN


PATTERN_03:
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,80
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,80
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,40
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,120
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,80
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,80
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,40
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,120
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,80
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,80
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,40
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,120
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,80
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,80
	INSTR2 $02,$90,$b1,79,127
	SET_VOLENV 3
	PLAY2 3,$5e,40
	INSTR2 $00,$80,$e0,46,127
	SET_VOLENV 4
	PLAY2 164,$59,40
	PLAY2 164,$59,80
RETURN


PATTERN_04:
	INSTR2 $00,$80,$80,104,127
	SET_VOLENV 2
	PLAY2 251,$78,255
	DELAY 65
	PLAY2 251,$78,255
	DELAY 65
	PLAY2 251,$78,255
	DELAY 65
	PLAY2 251,$78,80
	PLAY2 251,$78,80
	PLAY2 251,$78,80
	PLAY2 251,$78,80
RETURN


PATTERN_05:
	INSTR2 $00,$80,$80,104,127
	SET_VOLENV 2
	PLAY2 224,$78,255
	DELAY 65
	PLAY2 70,$7a,255
	DELAY 65
	PLAY2 251,$78,255
	DELAY 65
	PLAY2 251,$78,255
	DELAY 65
RETURN