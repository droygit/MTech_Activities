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
    listing(bitsid_registered),
    told,
    write('Bye Bye!'),nl.

%name_change(Bitsid,IName):-
%    nl,write("Your name is not matching with the BITS Id."),
%    nl,write("Do you want to update your name [0/1] ?"),
%    read(Option),
%    (Option =:= 1 ->
%        retract(bitsid_name_mapping(Bitsid,_)),
%        assertz(bitsid_name_mapping(Bitsid,IName));!
%    ).

app_status_command(BitsId,1):-
    (bits_score(BitsId,OMarks) ->
        nl,write("Congratulation! You got "), write(OMarks), write("% marks in BITSAT."),
        nl,write("You are selected.");
        nl,write("Sorry! Score Database doesn't have your BITS id."),
        nl,write("Please register your score.")
    ),
    nl.

app_status_command(BitsId,2):-
    %write("Please provide the payment reference number - "),nl,
    %read(PaymentRef),
    ((bits_payment(BitsId,PaymentRef)) ->
        nl,write("I confirm that your payment is done and registered with us."),
        nl,write("Payment reference - "), write(PaymentRef);
        nl,write("Sorry! I couldn't find any payment reference against this BITSID."),
        nl,write("Please register your payment reference.")
    ),
    nl.

app_status_command(BitsId,3):-
    ((bits_payment(BitsId,PaymentRef)) ->
        nl,write("Your payment is already done and registered with us."),
        nl,write("Payment reference - "), write(PaymentRef),
        bits_score(BitsId,Score),
        nl,write("Your score - "), write(Score);
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
        )
    ),
    nl.

command(1,IName,BitsId):-
    show_all_courses(),
    continue_menu(IName,BitsId).

command(2,IName,BitsId):-
    show_degree_info(IName),
    continue_menu(IName,BitsId),
    nl.

command(3,IName,BitsId):-
    info_application_procedure(),
    continue_menu(IName,BitsId),
    nl.

command(4,IName,BitsId):-
    write("Hi "),
    write(IName),
    write(". Your BITS id - "),write(BitsId),
    nl,write("Please choose one of the following options - "),nl,
    nl,write("1. Get your BITS Score"),
    nl,write("2. Get your payment status"),
    nl,write("3. Register your score and payment reference"),nl,
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3) ->
        app_status_command(BitsId,Option);
        nl,write("Incorrect option. Please try again.")
    ),
    continue_menu(IName,BitsId),
    nl.

command(5,IName,BitsId):-
    info_admission_procedure(),
    info_scholarship(),
    info_bitsat_score_analysis(),
    continue_menu(IName,BitsId),
    nl.

%command(6,IName,BitsId):-
%    continue_menu(IName,BitsId),
%    nl.

command(6,IName,BitsId):-
    write("Thanks for showing interest "),
    write(IName), write("("), write(BitsId), write(")"),
    write("! Talk to you later."),
    nl.

general_options:-
    write("Choose one of the follwing number to proceed -"),nl,
    write("1. View all offered courses"),nl,
    write("2. Eligible for suitable courses"),nl,
    write("3. Application process (Deadlines and Dates)"),nl,
    write("4. Application status"),nl,
    write("5. About admission"),nl,
    write("6. Quit"),nl.


continue_menu(IName,BitsId):-
    nl,nl,
    write("Welcome back "),
    write(IName), write("("), write(BitsId), write(")"),
    write(" to main menu. Please select the next topic - "),nl,
    general_options(),
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3 ; Option =:= 4 ; Option =:= 5 ; Option =:= 6 ; Option =:= 7) ->
        command(Option,IName,BitsId);
        write("Sorry "), write(IName),
        write("! Invalid command. Start from beginning.")
    ).


%Displays menu to user
main_menu:-
    write("Hi. I am a chatbot. "),nl,
    write("What is your name? "),nl,
    read_string(Name),
    nl,write("Please provide your BITS id - "),
    read(Bitsid),
    ((bitsid_registered(Bitsid)) ->
        format('Hi ~s. Your BITS id is already registered with us.',[Name]);
        write("I couldn't find your register id. I will register your BITS ID."),nl,
        assertz(bitsid_registered(Bitsid)),
        write("Next time I will recognise you using your BITS ID.")
    ),

    nl,nl,write("I can help you regarding follwoing topics - "),nl,
    general_options(),
    nl,write("Please choose one of the option now - "),
    read(Option),
    ((Option =:= 1 ; Option =:= 2 ; Option =:= 3 ; Option =:= 4 ; Option =:= 5 ; Option =:= 6 ; Option =:= 7) ->
        
        command(Option,Name,Bitsid);
        write("Sorry "), write(Name),
        write("! Invalid command. Start from beginning."),nl
    ),
    save.
