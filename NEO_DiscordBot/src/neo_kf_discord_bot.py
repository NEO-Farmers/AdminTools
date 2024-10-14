import datetime
import json
from operator import itemgetter
import os
import re

import discord

token = os.getenv("NEO_DISCORD_BOT_TOKEN")
config_file_name = os.getenv("NEO_DISCORD_BOT_CONFIG_FILE")
config = {}
config['guilds'] = {}

kill_msg_regex = re.compile(r"\[(?P<victim_name>.*)\]\(\<https://app.cftools.cloud/profile/(?P<victim_cftools_id>[a-z0-9]*)\>\) Pos: \<X: (?P<victim_pos_x>[0-9\.]*), Y: (?P<victim_pos_y>[0-9\.]*), Z: (?P<victim_pos_z>[0-9\.]*)\> got killed by \[(?P<murderer_name>.*)\]\(\<https://app.cftools.cloud/profile/(?P<murderer_cftools_id>[a-z0-9]*)\>\) Pos: \<X: (?P<murderer_pos_x>[0-9\.]*), Y: (?P<murderer_pos_y>[0-9\.]*), Z: (?P<murderer_pos_z>[0-9\.]*)\> \((?P<weapon>.*), (?P<range>[0-9.]*)m\)")

hit_msg_regex = re.compile(r"\[(?P<victim_name>.*)\]\(\<https://app.cftools.cloud/profile/(?P<victim_cftools_id>[0-9a-z].*)\>\) Pos: \<X: (?P<victim_pos_x>[0-9\.]*), Y: (?P<victim_pos_y>[0-9\.]*), Z: (?P<victim_pos_z>[0-9\.]*)\> got hit by \[(?P<murderer_name>.*)\]\(\<https://app.cftools.cloud/profile/(?P<murderer_cftools_id>[a-z0-9]*)\>\) Pos: \<X: (?P<murderer_pos_x>[0-9\.]*), Y: (?P<murderer_pos_y>[0-9\.]*), Z: (?P<murderer_pos_z>[0-9\.]*)\> \((?P<weapon>.*), (?P<distance>[0-9\.]*)m, (?P<damage>[0-9\.]*) damage, hitzone (?P<hitzone>.*)\)")

def process_kf_msg(kf_message):
    result = {}
    m = kill_msg_regex.match(kf_message)
    if m:
        result = m.groupdict()
        result['type'] = 'kill'
    
    m = hit_msg_regex.match(kf_message)
    if m:
        result = m.groupdict()
        result['type'] = 'hit'
    print(json.dumps(result, indent=2))
    return result

def load_config():
    global config
    try:
        if config_file_name != "":
            with open(config_file_name, "r") as configfile:
                config_data = configfile.read()
                if len (config_data) > 2: #{} is technically valid json
                    config_dict = json.loads(config_data)
                    # if no exception, store it
                    config = config_dict
                    print("loaded config:")
                    print(json.dumps(config, indent=2))
    except:
        print("unable to load config file")

def save_config():
    global config
    try:
        if config_file_name != "":
            with open(config_file_name, "w") as configfile:
                configfile.write(json.dumps(config, indent=2))
                print("saved config:")
                print(json.dumps(config, indent = 2))
                
    except:
        print("Unable to save config")


intents = discord.Intents.default()

intents.message_content = True

client = discord.Client(intents=intents)

@client.event
async def on_ready():
    print(f"We have logged in as {client.user}")
    print(f"Guilds: {client.guilds}")

@client.event
async def on_guild_join(guild):
    print(f"Bot now in guild: {guild}")

    config['guilds'][guild.id] = {}
    save_config()


    
@client.event
async def on_message(message):
    if message.author == client.user:
        print ("Ignoring self message")
        return

    print("message: ")
    print(message)
    print("content:")
    print(message.content)
    if message.mentions and len(message.mentions) == 1 and message.mentions[0].id == client.user.id:
        print("someone mentioned me")

        if "/help" in message.content:
            await message.channel.send(
                    "NEODiscordBot help:\n" + 
                    "     /help - this message\n"+
                    "     /neo_kf_listen - send in channel you wish bot to listen to killfeed in\n"+
                    "     /neo_kf_write - send in channel you wish bot to write processed results in\n"+
                    "     /neo_purge_leaders - generate a leader board message from current room\n"+
                    "     /neo_config - dump config in channel\n"+
                    "        note: this is currently hard-coded to start 10/12/2024 at 2pm EST"
                )
        # kill feed channel listen messages
        if "/neo_kf_listen" in message.content:
            print(f"listen request: {message}, content: {message.content}")
            print(f"listening on chennel id: {message.channel.id}")
            if message.guild.id not in config['guilds']:
                print("got this from a guild I don't know, skipping")
                return
            config['guilds'][message.guild_id]['kf_listen'] = message.channel.id
            await message.channel.send("NEODiscordBot KillFeed Cleaner listening here")
            save_config()


        if "/neo_kf_write" in message.content:
            print(f"listen request: {message}, content: {message.content}")
            print(f"listening on chennel id: {message.channel.id}")
            if message.guild.id not in config['guilds']:
                print("got this from a guild I don't know, skipping")
                return
            config['guilds'][message.guild_id]['kf_write'] = message.channel.id
            await message.channel.send("NEODiscordBot KillFeed Cleaner writing here")

            save_config()
        if "/neo_config" in message.content:
            await message.channel.send("```\njson.dumps(config, indent=2))\n```")
        if "/neo_purge_leaders" in message.content:
            print(f"got kill leader count request: {message}")
            await message.channel.send("NEODiscordBot: counting kill leaders")
            print(f"last message id: {message.channel.last_message_id}")
            purge_start = datetime.datetime(2024,10,12,18,0,0,0,datetime.timezone.utc)
            print(f"purge start: {purge_start} tz: {purge_start.tzname()}")
            count = {}
            async for m in message.channel.history(limit=None, after=purge_start):
                #print(f"mesage:{m}\n\n content:{m.content}{{endcontent}}")
                result = process_kf_msg(m.content)
                if not result:
                    print(f'failed to parse: {m.content}')
                    continue
                name = result['murderer_name']
                cftools_id = result['murderer_cftools_id']

                if cftools_id not in count:
                    count[cftools_id] = {'kills':0, 'names':[], 'headshots': 0, 'brainshots': 0}
                entry = count[cftools_id]
                if name not in entry['names']:
                    entry['names'].append(name)

                if result['type'] == 'kill':
                    entry['kills'] = entry['kills'] + 1
                if result['type'] == 'hit' and 'hitzone' in result:
                    hitzone = result['hitzone']
                    if hitzone == 'head':
                        entry['headshots'] = entry['headshots'] + 1
                    if hitzone == 'brain':
                        entry['brainshots'] = entry['brainshots'] + 1
            with open("neo_kf_leaders.txt","w") as outfile:
                outfile.write(json.dumps(count, indent=2))

            tuples = []
            for x in count:
                tuples.append((x, count[x]['kills']))
            print(tuples)
            sorted_tuples = sorted(tuples,key=itemgetter(1), reverse=True)
            print(sorted_tuples)
            kill_list = []
            with open("neo_kf_leaders.txt", "w") as outfile:

                outfile.write(f"name                           | kills | heads | brains\n")
                outfile.write( "-------------------------------|-------|-------|-------\n")
                for x in sorted_tuples:
                    entry = count[x[0]]
                    outfile.write(f"{entry['names'][0]:30} | {entry['kills']:5} | {entry['headshots']:5} | {entry['brainshots']:6}\n")
            with open("neo_kf_leaders.txt", "r") as infile:
                await message.channel.send("result_file:", file=discord.File(infile))





load_config()

client.run(token)

