#define BAUDRATE 9600
#define BUFFER_STRING 200

#define CNF_PORTB 0x20
#define INI_PORTB 0x00

#define CNF_PORTD 0x10
#define INI_PORTD 0x10
#define MASK_LED_ON   0xEF
#define MASK_LED_OFF  0x10

#define MASK_LED_CLOCK 0x20
#define TAREA_ACT 1
#define TAREA_DES 0

#define PERIOD 500 // [ms]
#define INI_TNT 0xFFF0

float volt = 0, factVolt = 1.1/1023.0, temp = 0;
volatile unsigned int k = 0;
String bufferString = "";

struct {
  unsigned sendData :  TAREA_ACT;
  unsigned actionClock : TAREA_ACT;
  unsigned led1On : TAREA_ACT;
  unsigned led1Off : TAREA_ACT;
} tareas;
