////////////////////////////////////////////////////////////////////////////////
//File:	main.c
//Author:	Chad Revel
//This is the command line interface header file of the sixth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#ifndef CLICOMMANDS_H_
#define CLICOMMANDS_H_
#include <asf.h>
#include <FreeRTOS.h>
#include <FreeRTOS_CLI.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

portBASE_TYPE prvTaskStatsCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
//lab 6 cli commands
static const char taskListHdr[] = "Name\t\t\tStat\tPri\tS/Space\tTCB\r\n";
static const xCommandLineInput xTaskStatsCommand = {"task-stats", "task-stats: Displays a table of task state information\r\n", prvTaskStatsCommand, 0};

#endif /* CLICOMMANDS_H_ */