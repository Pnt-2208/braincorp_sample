//=============================================================================
// FILE: test.c
//
// DESCRIPTION:
// execute a test on the code sample
//
// AUTHORS:
//   Brooke Wallace <brookbot@yahoo.com> 2017,02
//
// Copyright 2017, Brooke Wallace, All rights reserved.
//=============================================================================

#include <features.h>
#ifdef __USE_POSIX
   #include <stdint.h>
#else
   #error (This code sample requires a POSIX system)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=============================================================================
// Types
//=============================================================================
// Some standard types I like to use
typedef unsigned char byte;
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int16_t int16;
typedef uint16_t uint16;
typedef float float32;
typedef double float64;

#include <assert.h>
#define ASSERT(x)\
    assert(x);

#define LOG_LEVEL_DEBUG 10
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL 100

#define DEBUG(...)\
   printf("[DEBUG][%d][%s] ", __LINE__, __FILE__);\
   printf(__VA_ARGS__);

#define ERROR(...)\
   printf("[ERROR][%d][%s] ", __LINE__, __FILE__);\
   printf("Error: " __VA_ARGS__);

#if __BYTE_ORDER != __LITTLE_ENDIAN
   #error(__BIG_ENDIAN is not supported)
#endif

// message packet header and display message
typedef struct {
   byte id;
   byte type;
   uint16 len;
   byte data;
} message_header;

// motor message type
typedef struct {
   byte id;
   byte type;
   uint16 len;
   float32 forward_back;
   float32 left_right;
} motor_msg_type;

//=============================================================================
// Functions
//=============================================================================
int display_message(message_header* msg);
int update_motor(message_header* msg);

int dispatch(message_header* msg);

void message_show(message_header* msg);

//=============================================================================
// Global vars
//=============================================================================
uint8 display_packet[] = {0x1, 0x34, 0x05, 0x0, 0x48, 0x65, 0x6c, 0x6c, 0x6f};
uint8 motor_packet[] = {0x2, 0x80, 0x08, 0x0, 0x0, 0x0, 0x80, 0x3f
                     , 0x0, 0x0, 0x0, 0xbf}; 
//uint8 invalid_packet[] = {0x3, 0x22, 0x4, 0x0};

byte* test_data[] = {
     display_packet
   , motor_packet
//   , invalid_packet
   , 0
};

// message types
#define MSG_DISPLAY 0x34
#define MSG_MOTOR   0x80

#include <limits.h>
#define MAX_MESSAGE_SIZE INT_MAX

//=============================================================================
// FUNCTION: main
//
// DESCRIPTION:
// test application entry point
//
// RETURN: 
// 0 - succes
// 1 - failure
//=============================================================================
int main(int argc, char** argv, char** env) {

   printf("Begin tests...\n");

   int total_fail = 0;
   
   int i=0;
   while(test_data[i]) {
      message_header* message = (message_header*)test_data[i];
      int result = dispatch(message);
      if(!result) {
          ERROR("Test[%d] Failed with %d\n", i, result);
          total_fail++;
      }
      i++;
   }

   int total_tests = i;
   int total_pass = i - total_fail;

   DEBUG("=== SUMMARY ===\n");
   DEBUG("Total Pass: %d\n", total_pass);
   DEBUG("Total Fail: %d\n", total_fail);
   DEBUG("Total Tests: %d\n", total_tests);

   return total_fail;
}

//=============================================================================
// FUNCTION: dispatch <msg>
//
// DESCRIPTION:
// Brain Corp coding sample
// a code example written in C that dispatches messages based upon their type.
// There are two message types "display" and "motor".  These two message types
// are mixed into the same packet format and the function that needs to be
//  written needs to distinguish between these two messages and call the
// appropriate function.  Display messages must be passed to the function
// called display_message() and motor messages must be passed to a function
// called update_motor().
//
// RETURN:
// 0 - failure
// 1 - success 
//=============================================================================
int dispatch(message_header* msg) {

   if (LOG_LEVEL) {
      message_show(msg);
   }
   switch(msg->type) {
      case MSG_DISPLAY: {
         display_message(msg);
         break;
      }
      case MSG_MOTOR: {
         update_motor(msg);
         break;
      }
      default: {
         ERROR("Unknown message type 0x%x\n", msg->type);
         return 0;
      }
   }

   return 1;
}

//=============================================================================
// FUNCTION: display_message <msg>
//
// DESCRIPTION:
// Processes the ascii text "display message" <msg> and 
// writes the text to the display
//
// RETURN:
// 0 - failure
// 1 - sucess
//=============================================================================
int display_message(message_header* msg) {

   ASSERT(msg->type == MSG_DISPLAY);
   ASSERT(msg->len <= MAX_MESSAGE_SIZE);

   byte* text = (byte*)malloc(msg->len + 1);
   if (!text) {
      ERROR("Failed to allocate %d bytes to display message\n", msg->len +1);
      return 0;
   }

   memcpy(text, &msg->data, msg->len);
   text[msg->len +1]=0; // null terminate string for display

   printf("Display message: %s\n", text);

   free(text);   
   return 1;
}

//=============================================================================
// FUNCTION: update_motor <msg>
//
// DESCRIPTION:
// Process the motor update <msg> and does nothing right now.
//
// RETURN:
// 0 - failure
// 1 - success
//=============================================================================
int update_motor(message_header* msg) {
   ASSERT(msg);
   ASSERT(msg->type == MSG_MOTOR);

   motor_msg_type* motor_cmd = (motor_msg_type*)msg;

   DEBUG("Update Motor: %f %f\n"
         , motor_cmd->forward_back, motor_cmd->left_right);
   return 1;
}

//=============================================================================
// FUNCTION: message_show <msg>
//
// DESCRIPTION:
// dumps <msg> to stdout for debug
//
// RETURN: None
//=============================================================================
void message_show(message_header* msg) {
   ASSERT(msg);

   DEBUG("Message[%d]: type: 0x%x, len: %d\n", msg->id, msg->type, msg->len);
//   hexdump(msg, msg->len, "");
}
