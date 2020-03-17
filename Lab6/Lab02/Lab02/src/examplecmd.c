/* Note: To use the VTaskList Command you must modify the FreeRTOSConfig.h
 * to change: 
 * #define configUSE_TRACE_FACILITY                1
 * #define configUSE_STATS_FORMATTING_FUNCTIONS    1 */
static const char taskListHdr[] = "Name\t\t\tStat\tPri\tS/Space\tTCB\r\n";

static const xCommandLineInput xTaskStatsCommand = {"task-stats",
            "task-stats: Displays a table of task state information\r\n",
            prvTaskStatsCommand,
            0};

			
// Note: You should check to make sure everything fits in the buffer you are 
// sending back. This example does not...
portBASE_TYPE prvTaskStatsCommand(int8_t *pcWriteBuffer, 
                                  size_t xWriteBufferLen,
                                  const int8_t *pcCommandString)
{
    sprintf(pcWriteBuffer, taskListHdr);
    pcWriteBuffer += strlen(taskListHdr);
    vTaskList(pcWriteBuffer);

    return pdFALSE;
}
