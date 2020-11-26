% A simple course advisor system
% The user interface

:- dynamic continue_menu/1.
:- dynamic show_degree_info/1.
:- dynamic app_status_command/2.


read_string(OString):-
    current_input(Input),
    read_line_to_codes(Input, Codes),
    string_codes(OString, Codes).

% Initialization procedures
start:-
    reconsult('bitsdb.pl'),nl,
    reconsult('bitsrules.pl'),nl,
    reconsult('bitsinfo.pl'),nl,
    main_menu.

% Saves new temporary info for use by inference engine
save:-
    write('Saving data'),nl,
    tell('bitsdb.pl'),
    listing(bits_payment),
    listing(bits_score),
    told,
    write('Bye Bye!'),nl.

app_status_command(BitsId,1):-
    (bits_score(BitsId,OMarks) ->
        nl,write("Congratulation! You got "), write(OMarks), write(" marks"),
        nl,write("You are selected.");
        nl,write("Sorry! You are not selected.")
    ),
    nl.

app_status_command(BitsId,2):-
    write("Please provide the payment reference number - "),nl,
    read(PaymentRef),
    ((bits_payment(BitsId,PaymentRef)) ->
        nl,write("I confirm that your payment is done and registered with us.");
        nl,write("Sorry! I couldn't find the BITSID and reference")
    ),
    nl.

app_status_command(BitsId,3):-
    nl,write("Please provide the payment reference number - "),nl,
    read(PaymentRef),
    nl,write("Please provide your BITS Score: "),
    read(BitsScore),
    (is_eligible(BitsScore) ->
        assertz(bits_payment(BitsId,PaymentRef)),
        assertz(bits_score(BitsId,BitsScore)),
        nl,write("Your BITSID = "), write(BitsId), write(" and Payment Reference No.= "),write(PaymentRef),nl,
        write("I will update the data base once you are done with your quiries.");
        write("Sorry. You couldn't clear the cutoff marks. I can't register your payment information.")
    ),
    nl.

command(1,IName):-
    show_all_courses(),
    continue_menu(IName).

command(2,IName):-
    write("Please choose one of the following courses - "),nl,
    show_degree_info(IName),
    continue_menu(IName),
    nl.

command(3,IName):-
    info_application_procedure(),
    continue_menu(IName),
    nl.

command(4,IName):-
    write("Hi "),
    write(IName),
    write(". Please provide your BITS id - "),nl,
    read(Bitsid),
    format('Just to confirm, your BITS ID = ~s',[Bitsid]),
    nl,write("Please choose one of the following options - "),nl,
    nl,write("1. Get your BITS Score"),
    nl,write("2. If you already paid the amount"),
    nl,write("3. If you paid the amount and want to register your BITS ID"),nl,
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3) ->
        app_status_command(Bitsid,1);
        nl,write("Incorrect option. Please try again.")
    ),
    continue_menu(IName),
    nl.

command(5,IName):-
    info_admission_procedure(),
    info_scholarship(),
    info_bitsat_score_analysis(),
    continue_menu(IName),
    nl.

command(6,IName):-
    continue_menu(IName),
    nl.

command(7,IName):-
    write("Thanks for showing interest "),
    write(IName),
    write("! Talk to you later."),
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


continue_menu(IName):-
    nl,nl,
    write("Welcome back "),
    write(IName),
    write(" to main menu. Please select the next topic - "),nl,
    general_options(),
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3 ; Option =:= 4 ; Option =:= 5 ; Option =:= 6 ; Option =:= 7) ->
        command(Option,IName);
        write("Sorry "), write(IName),
        write("! Invalid command. Start from beginning.")
    ),
    nl.

%Displays menu to user
main_menu:-
    write("What is your name? "),nl,
    read_string(Name),
    format('Hi ~s',[Name]),
    write(". I am a chatbot. "),
    write("I can help you regarding follwoing topics - "),nl,
    general_options(),
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3 ; Option =:= 4 ; Option =:= 5 ; Option =:= 6 ; Option =:= 7) ->
        command(Option,Name);
        write("Sorry "), write(Name),
        write("! Invalid command. Start from beginning.")
    ),
    nl,
    save.
