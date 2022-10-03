#include <string.h>
#include <concord/discord.h>
#include "main.h" 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_guild_application_command params = {
        .name = "ping",
        .description = "Ping command!"
    };
    discord_create_guild_application_command(client, event->application->id,
                                             GUILD_ID, &params, NULL);
}
 
void on_interaction(struct discord *client, const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return; /* return if interaction isn't a slash command */
 
    if (strcmp(event->data->name, "ping") == 0) {
          struct discord_interaction_response params = {
                .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
                .data = &(struct discord_interaction_callback_data){
                      .content = "pong"
                }
          };
          discord_create_interaction_response(client, event->id,
                                              event->token, &params, NULL);
    }
}

void print_usage(void) {
    printf("\n\nThis is a bot to demonstrate an easy to make 8ball response "
           "to a question.\n"
           "1. type '8ball (question)' in chat\n"
           "\nTYPE ANY KEY TO START BOT\n");
}

void eight_ball(struct discord *client, const struct discord_message *event) {
    if (event->author->bot) return;

    /* List of 8ball phrases/responses */
    char *phrases[] = {
        ":green_circle: It is certain.",
        ":green_circle: It is decidedly so.",
        ":green_circle: Without a doubt.",
        ":green_circle: Yes definitely.",
        ":green_circle: You may rely on it.",
        ":green_circle: As I see it, yes.",
        ":green_circle: Most likely.",
        ":green_circle: Outlook good.",
        ":green_circle: Yes.",
        ":green_circle: Signs Point to Yes.",
        ":yellow_circle: Reply hazy, try again.",
        ":yellow_circle: Ask again later.",
        ":yellow_circle: Better not tell you now.",
        ":yellow_circle: Cannot predict now.",
        ":yellow_circle: Concentrate and ask again.",
        ":red_circle: Don't count on it.",
        ":red_circle: My reply is no.",
        ":red_circle: My sources say no.",
        ":red_circle: Outlook not so good.",
        ":red_circle: Very doubtful.",
    };
    /* random index to phrases array */
    int answer = rand() % (sizeof(phrases) / sizeof(*phrases));

    struct discord_embed embeds[] = { {
        .title = ":8ball: 8-Ball",
        .description = phrases[answer],
    } };

    struct discord_create_message params = {
        .embeds =
            &(struct discord_embeds){
                .size = sizeof(embeds) / sizeof *embeds,
                .array = embeds,
            },
    };
    discord_create_message(client, event->channel_id, &params, NULL);
}

char *SPAM[] = {
    "Yes I love to spam", // 1
    "Do you?", // 2
    "ROFL", // 3
    "What are you going to do about it?", // 4
    "Are you going to stop me?", // 5
    "Good luck with that.", // 6
    "Many have tried but..", // 7
    "They all fail.", // 8
    "What makes you think", // 9
    "It should be any different with you?" // 10
};

void on_spam_async(struct discord *client, const struct discord_message *event) {
    if (event->author->bot) return;

    for (size_t i = 0; i < 10; ++i) {
        struct discord_create_message params = { .content = SPAM[i] };
        discord_create_message(client, event->channel_id, &params, NULL);
    }
}

void on_spam_sync(struct discord *client, const struct discord_message *event) {
    if (event->author->bot) return;

    for (size_t i = 0; i < 10; ++i) {
        struct discord_ret_message ret = { .sync = DISCORD_SYNC_FLAG };
        struct discord_create_message params = { .content = SPAM[i] };
        discord_create_message(client, event->channel_id, &params, &ret);
    }
}
 
int main(void) {
    struct discord *client = discord_init(TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);
    discord_set_on_command(client, ">8ball", &eight_ball);
      discord_set_on_command(client, ">spam-async", &on_spam_async);
    discord_set_on_command(client, ">spam-sync", &on_spam_sync);
    discord_add_intents(client, INTENTS_ALL);
    discord_run(client);
}