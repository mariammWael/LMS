#ifndef PREFERENCES_H
#define PREFERENCES_H

#define PREF_ID_MAX 30
#define PREF_MEMBER_MAX 30
#define PREF_SUGGESTION_MAX 120

typedef struct {
    char preferenceID[PREF_ID_MAX];
    char memberID[PREF_MEMBER_MAX];
    char suggestion[PREF_SUGGESTION_MAX];
} Preferences;

void preferences_init(Preferences *p,
                      const char *prefID,
                      const char *memberID);

void preferences_generateRecommendations(Preferences *p,
                                          const char *memberID);

void preferences_recordFeedback(Preferences *p,
                                const char *memberID,
                                const char *feedback);

#endif /* PREFERENCES_H */
