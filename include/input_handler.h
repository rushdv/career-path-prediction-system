#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Robust string input (safely reads up to max_len - 1 characters, strips newline) */
void getStringInput(char *buffer, int max_len);

/* Robust float input, prompts until a valid float between min and max is given */
float getFloatInput(float min, float max);

/* Robust integer input, prompts until a valid int is given */
int getIntInput(void);

/* Interactive arrow-key menu selection
 * Returns the index (0-based) of the selected option, or -1 if aborted/error.
 */
int getMenuSelection(const char *title, const char *subtitle, const char **options, int num_options);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_HANDLER_H */
