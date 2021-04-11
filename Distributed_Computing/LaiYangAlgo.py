#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
@author: Debojyoti Roy
"""
import argparse
from enum import Enum
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
kPosSendWhiteMsgHistory = 7
kPosRecvWhiteMsgHistory = 8
kPosTransitMsg = 9

kTotalNumProcess = 2
process_id = 0

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
        [eEvents.TIME, 6, 2, 0, 200, 'NA'],
        [eEvents.RECEIVE_WHITE_MSG, 6.5, 1, 2, 30, 'm12-3'],
        [eEvents.TIME, 7, 2, 0, 230, 'NA'],
        [eEvents.SEND_WHITE_MSG, 8.5, 2, 1, 20, 'm21-2'],
        [eEvents.TIME, 8, 2, 0, 210, 'NA'],
        [eEvents.TIME, 9, 2, 0, 210, 'NA']]
]

def print_input():
    #TODO: Read from file and create 'all_process_events' array
    for process in all_process_events:
        print ('++++ Process Events ++++')    
        df = pandas.DataFrame(process, columns=column_name)
        print (df)
        print ('+++++++++++++++++++++++++\n')
    

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
    col = column_name
    col.append('Process_State')
    col.append('White_Msg_History')
    col.append('Channel_info')
    col.append('Transit_messages')
    
    for event in process_events:
        #print('Debug! Event = ', event)
        event.append(current_state)
        if event[kPosEventDescription] == eEvents.TIME:
            if len(white_msg_history) == 0:
                event.append('None')
            else:
                event.append(white_msg_history)
            event.append(channels)
            event.append(transit)
        elif event[kPosEventDescription] == eEvents.SEND_WHITE_MSG:
            # Message is sent before taking local snapshot
            if current_state == eProcesState.WHITE:
                msg = ['S', event[kPosMessageId], event[kPosToPricessId], event[kPosTransactionAmount]]
                white_msg_history.append(msg)
                #print('Debug! white_msg_history = ', white_msg_history)
            event.append(white_msg_history)
            event.append(channels)
            event.append(transit)
        elif event[kPosEventDescription] == eEvents.SEND_RED_MSG:
            # Message is sent after taking local snapshot
            pass
        elif event[kPosEventDescription] == eEvents.RECEIVE_WHITE_MSG:
            msg = ['R', event[kPosMessageId], event[kPosFromPricessId], event[kPosTransactionAmount]]
            white_msg_history.append(msg)
            #print('Debug! white_msg_history = ', white_msg_history)
            event.append(white_msg_history)
            event.append(channels)
            event.append(transit)
            pass
        elif event[kPosEventDescription] == eEvents.RECEIVE_RED_MSG:
            # This process should take its local snapshot
            if current_state == eProcesState.WHITE:
                # When 'white' process receives a red message,
                # it records its local snapshot before processing the message
                pass
            pass
        elif event[kPosEventDescription] == eEvents.TAKE_SNAPSHOT:
            # Taking local snapshot
            # At this time, this process turned red
            current_state = eProcesState.RED
            event[kPosProcessState] = current_state
            event.append(white_msg_history)
            event.append(channels)
            event.append(transit)
            pass
        else:
            print('Error! Unknown event = ', event[kPosEventDescription])
            
        print('\n\nDebug! Append event = ', event)
        
        final_output.append(event)
        df = pandas.DataFrame(final_output, columns=col)
        print (df)
    

    df = pandas.DataFrame(final_output, columns=col)
    print('\n\nFinal Output - ')
    print (df)
            
        
def start_Lai_Yang(process_id):
    print_input()
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
        start_Lai_Yang(process_id)
    
    print('---- Program Ends ----')