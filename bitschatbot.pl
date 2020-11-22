% A simple course advisor system
% The user interface

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

command(1):-
    show_all_courses(),
    continue_menu().

command(2):-
    write("Please choose one of the following courses - "),nl,
    show_degree_info(),
    continue_menu(),
    nl.

command(3):-
    write("Not implemented yet."),nl,
    continue_menu(),
    nl.

command(4):-
    write("Not implemented yet."),nl,
    continue_menu(),
    nl.

command(5):-
    write("Not implemented yet."),nl,
    continue_menu(),
    nl.

command(6):-
    write("Not implemented yet."),nl,
    continue_menu(),
    nl.

command(7):-
    write("Thanks for showing interest! Talk to you later. Bye!"),
    nl.

general_options:-
    write("Choose one of the follwing number to proceed -"),nl,
    write("1. View all offered courses"),nl,
    write("2. Eligible for suitable courses"),nl,
    write("3. Application process"),nl,
    write("4. Application status"),nl,
    write("5. About admission"),nl,
    write("6. Continue"),nl,
    write("7. Quit"),nl.


continue_menu:-
    write("Hi there. Please select the next topic - "),nl,
    general_options(),
    read(Option),
    command(Option),
    nl.

%Displays menu to user
main_menu:-
    write("Hi there. I am a chatbot. I can help you regarding follwoing topics - "),nl,
    general_options(),
    read(Option),
    command(Option),
    nl.
