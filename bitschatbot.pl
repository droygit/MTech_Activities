% A simple course advisor system
% The user interface

:- dynamic continue_menu/1.
:- dynamic show_degree_info/1.

% Initialization procedures
start:-
    reconsult('bitstemp.pl'),nl,
    reconsult('bitsrules.pl'),nl,
    reconsult('bitscourseinfo.pl'),nl,
    main_menu.

% Saves new temporary info for use by inference engine
save:-
    write('Saving data'),nl,
    tell('bitstemp.pl'),
    listing(grade),
    listing(interest),
    told,
    write('Done'),nl.

command(1,IName):-
    show_all_courses(),
    continue_menu(IName).

command(2,IName):-
    write("Please choose one of the following courses - "),nl,
    show_degree_info(IName),
    continue_menu(IName),
    nl.

command(3,IName):-
    write("Not implemented yet."),nl,
    continue_menu(IName),
    nl.

command(4,IName):-
    write("Not implemented yet."),nl,
    continue_menu(IName),
    nl.

command(5,IName):-
    write("Not implemented yet."),nl,
    continue_menu(IName),
    nl.

command(6,IName):-
    write("Not implemented yet."),nl,
    continue_menu(IName),
    nl.

command(7,IName):-
    write("Thanks for showing interest! Bye "),
    write(IName),
    write("! Talk to you later."),
    nl.

read_name(OString):-
    current_input(Input),
    read_line_to_codes(Input, Codes),
    string_codes(OString, Codes).

general_options:-
    write("Choose one of the follwing number to proceed -"),nl,
    write("1. View all offered courses"),nl,
    write("2. Eligible for suitable courses"),nl,
    write("3. Application process"),nl,
    write("4. Application status"),nl,
    write("5. About admission"),nl,
    write("6. Continue"),nl,
    write("7. Quit"),nl.


continue_menu(IName):-
    write("Welcome back "),
    write(IName),
    write(" to main menu. Please select the next topic - "),nl,
    general_options(),
    read(Option),
    command(Option,IName),
    nl.

%Displays menu to user
main_menu:-
    write("What is your name? "),nl,
    read_name(Name),
    format('Hi ~s',[Name]),
    write(". I am a chatbot. "),
    write("I can help you regarding follwoing topics - "),nl,
    general_options(),
    read(Option),
    command(Option,Name),
    nl.
