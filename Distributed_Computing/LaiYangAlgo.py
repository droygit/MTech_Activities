#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
@author: Debojyoti Roy
"""
import argparse
from enum import Enum
from tabulate import tabulate
import pandas

class eProcesState(Enum):
    WHITE = 0
    RED = 1

class eEvents(Enum):
    TIME = 0
    SEND_WHITE_MSG = 1
    SEND_RED_MSG = 2
    RECEIVE_WHITE_MSG = 3
    RECEIVE_RED_MSG = 4
    TAKE_SNAPSHOT = 5

kPosEventDescription = 0
kPosTimestamp = 1
kPosFromPricessId = 2
kPosToPricessId = 3
kPosTransactionAmount = 4
kPosMessageId = 5
kPosProcessState = 6
kPosWhiteMsgHistory = 7
kPosSnapshot = 8
#kPosChannelInfo = 8
#kPosTransitMsg = 9

kTotalNumProcess = 2
process_id = 0

snapshot = []

column_name = ['Event_Decription', 'Timestamp', 'Pricess_id', 'Process_Id', 'Transaction', 'Message_id']
all_process_events = [
        #Process 1
        [[eEvents.TIME, 1, 1, 0, 800, 'NA'],
        [eEvents.SEND_WHITE_MSG, 1.5, 1, 2, 20, 'm12-1'],
        [eEvents.TIME, 2, 1, 0, 780, 'NA'],
        [eEvents.SEND_WHITE_MSG, 2.5, 1, 2, 30, 'm12-2'],
        [eEvents.TAKE_SNAPSHOT, 3, 1, 0, 750, 'NA'],
        [eEvents.SEND_RED_MSG, 3.5, 1, 2, 10, 'm12-3'],
        [eEvents.TIME, 4, 1, 0, 740, 'NA'],
        [eEvents.TIME, 5, 1, 0, 740, 'NA'],
        [eEvents.TIME, 6, 1, 0, 740, 'NA'],
        [eEvents.RECEIVE_WHITE_MSG, 6.5, 2, 1, 30, 'm21-1'],
        [eEvents.TIME, 7, 1, 0, 770, 'NA'],
        [eEvents.TIME, 8, 1, 0, 770, 'NA'],
        [eEvents.TIME, 8, 1, 0, 770, 'NA'],
        [eEvents.RECEIVE_WHITE_MSG, 8.5, 2, 1, 20, 'm21-2'],
        [eEvents.TIME, 9, 1, 0, 790, 'NA']],
        #Process 2
        [[eEvents.TIME, 1, 2, 0, 200, 'NA'],
        [eEvents.TIME, 2, 2, 0, 200, 'NA'],
        [eEvents.RECEIVE_WHITE_MSG, 2.5, 1, 2, 20, 'm12-1'],
        [eEvents.TIME, 3, 2, 0, 220, 'NA'],
        [eEvents.TIME, 4, 2, 0, 220, 'NA'],
        [eEvents.SEND_WHITE_MSG, 6.5, 2, 1, 30, 'm21-1'],
        [eEvents.TIME, 5, 2, 0, 190, 'NA'],
        [eEvents.RECEIVE_RED_MSG, 5.5, 1, 2, 10, 'm12-2'],
        [eEvents.SEND_RED_MSG, 5.6, 2, 1, 0, 'm21-2'],
        [eEvents.TIME, 6, 2, 0, 200, 'NA'],
        [eEvents.RECEIVE_WHITE_MSG, 6.5, 1, 2, 30, 'm12-3'],
        [eEvents.TIME, 7, 2, 0, 230, 'NA'],
        [eEvents.SEND_WHITE_MSG, 8.5, 2, 1, 20, 'm21-3'],
        [eEvents.TIME, 8, 2, 0, 210, 'NA'],
        [eEvents.TIME, 9, 2, 0, 210, 'NA']]
]

def print_input():
    #TODO: Read from file and create 'all_process_events' array
    for process in all_process_events:
        print ('\n\n++++ Process-{0} Events ++++'.format(process[0][kPosFromPricessId]))
        df = pandas.DataFrame(process, columns=column_name)
        print(tabulate(df, headers = 'keys', tablefmt = 'psql'))
    

def processing_events(process_id):
    process_events = all_process_events[process_id-1]
    current_state = eProcesState.WHITE
    white_msg_history = []
    channels = [] # Keep track of Cij
    transit = []  # Keep track of transit(LSi, LSj)
    for i in range(kTotalNumProcess):
        channels.append(0)
        transit.append(0)
    
    final_output = []
    isRedMsgSend = True
    keepHistory = True
    col = list(column_name)
    col.append('Process_State')
    col.append('White_Msg_History')
    col.append('Snapshot')
    col.append('Transit')
    
    event = []
    
    for singleEvent in process_events:
        #print('\n\nDebug! Event = ', event)
        
        event = singleEvent[:]
        event.append(current_state) # Place holder of current state. Will be updated with appropriate value.
        event.append([])            # Place holder of White message history
        event.append([])            # Place holder for Snapshot
        event.append([])            # Place holder for Transit

        if isRedMsgSend == True:
            current_state = eProcesState.WHITE
        if keepHistory == False:
            white_msg_history.clear()

        
        if event[kPosEventDescription] == eEvents.TIME:
            #if len(white_msg_history) == 0:
            #    event[kPosWhiteMsgHistory] = []
            #else:
            #    event[kPosWhiteMsgHistory] = white_msg_history [:]
            if current_state == eProcesState.RED:
                event[kPosWhiteMsgHistory] = white_msg_history [:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
            keepHistory = True
        elif event[kPosEventDescription] == eEvents.SEND_WHITE_MSG:
            # Message is sent before taking local snapshot
            #if current_state == eProcesState.WHITE:
            msg = ['S', event[kPosMessageId], event[kPosToPricessId], event[kPosTransactionAmount]]
            white_msg_history.append(msg)
                #print('Debug! white_msg_history = ', white_msg_history)
            #event[kPosWhiteMsgHistory] = white_msg_history[:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
            keepHistory = True
        elif event[kPosEventDescription] == eEvents.SEND_RED_MSG:
            # Message is sent after taking local snapshot
            isRedMsgSend = True
            keepHistory  = False
            event[kPosWhiteMsgHistory] = white_msg_history[:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
        elif event[kPosEventDescription] == eEvents.RECEIVE_WHITE_MSG:
            msg = ['R', event[kPosMessageId], event[kPosFromPricessId], event[kPosTransactionAmount]]
            white_msg_history.append(msg)
            #print('Debug! white_msg_history = ', white_msg_history)
            #event[kPosWhiteMsgHistory] = white_msg_history[:]
            if current_state == eProcesState.RED:
                event[kPosWhiteMsgHistory] = white_msg_history [:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
            keepHistory = True
        elif event[kPosEventDescription] == eEvents.RECEIVE_RED_MSG:
            # This process should take its local snapshot
            # When 'white' process receives a red message,
            # it records its local snapshot before processing the message
            current_state = eProcesState.RED
            event[kPosWhiteMsgHistory] = white_msg_history[:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
            keepHistory = True # TODO: Should I clear ?
            isRedMsgSend = False
        elif event[kPosEventDescription] == eEvents.TAKE_SNAPSHOT:
            # Taking local snapshot
            # At this time, this process turned red
            current_state = eProcesState.RED
            isRedMsgSend = False
            #event[kPosWhiteMsgHistory] = white_msg_history[:]
            #event[kPosChannelInfo] = channels[:]
            #event[kPosTransitMsg] = transit[:]
            keepHistory = True # Send Red message then clear the history
        else:
            print('Error! Unknown event = ', event[kPosEventDescription])
        
        event[kPosProcessState] = current_state
        #print('Debug! Append event = ', event)
        
        final_output.append(list(event))
        
        
    pandas.set_option('display.max_colwidth', 500)        
    df = pandas.DataFrame(final_output, columns=col)
    print('\n\nFinal Output - ')
    print(tabulate(df, headers = 'keys', tablefmt = 'psql'))
    
    
def start_Lai_Yang(process_id):
    processing_events(process_id)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-p','--pid', help='Process id', required=True)
    args = parser.parse_args()
    print(args.pid)
    process_id = int(args.pid)
    if process_id <= 0:
        print('Error! Process id can\'t be less or equal to zero.')
    elif process_id != 1 and process_id != 2:
        print('Error! Process id should be either 1 or 2.')
    else:
        print_input()
        start_Lai_Yang(1)
        start_Lai_Yang(2)
        print_input()

    print('---- Program Ends ----')