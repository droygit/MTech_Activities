:- dynamic guide_course/1.

show_course(IDegreeName,ICourseName):-
    write("\t"),
    write(IDegreeName),
    write(" in "),
    write(ICourseName),
    nl.

undergraduate_courses:-
    write("Undergraduate courses are - "),nl,
    show_course("B.E.", "Civil Engineering"),
    show_course("B.E.", "Chemical Engineering"),
    show_course("B.E.", "Electrical and Electronics Engineering"),
    show_course("B.E.", "Mechanical Engineering"),
    show_course("B.E.", "Computer Science").

postgraduate_courses:-
    write("Postgraduate courses are - "),nl,
    show_course("M.E.", "Chemical (with specialization in Petroleum Engineering)"),
    show_course("M.E.", "Communication Engineering"),
    show_course("M.E.", "Embedded Systems"),
    show_course("M.E.", "Computer Science").


show_all_courses:-
    undergraduate_courses(),
    postgraduate_courses(),
    nl.

subject_name(ISubject):-
    write(ISubject),
    write(": ").

guide_course(1):-
    write("Please proved obtained marks in Class XII examination for follwing subjects - "),nl,
    subject_name("Mathematics"),
    read(MathMarks),
    subject_name("Physics"),
    read(PhyMarks),
    subject_name("Chemistry"),
    read(ChemMarks),
    subject_name("English"),
    read(EngMarks),
    subject_name("Total Marks"),
    read(TotalMarks),

    ( 
        (TotalMarks < 60) ->
        write("Sorry! You are not eligible for any of the courses because Total Marks is less than 60%.");
        (
        (EngMarks < 50) ->
            write("Sorry! You are not eligible for any of the courses because English is less than 50%.");
            (
            (MathMarks =< 70) ->
                write("Sorry! You are not eligible for any of the courses because Mathematics is less than 70%.");
                (
                (PhyMarks < 70) ->
                    write("Sorry! You are not eligible for any of the courses because Physics is less than 60%.");
                    write("\tSuitable course(s) is/are -"),nl,
                    (
                    (PhyMarks >= 70 , PhyMarks < 80) ->
                        show_course("B.E.", "Mechanical Engineering"),
                        show_course("B.E.", "Civil Engineering"),
                        show_course("B.E.", "Computer Science");
                        show_course("B.E.", "Electrical and Electronics Engineering")
                    ),
                    (
                    (ChemMarks > 70) ->
                        show_course("B.E.", "Chemical Engineering")
                    )
                )
            )
        )
    ),
    write("Done!"),
    nl.

guide_course(2) :-
    write("Please provide the following information - "),

    nl.

show_degree_info:-
    write("1. B.E."),nl,
    write("2. M.E."),nl,
    read(Option),
    (
        (Option > 2 ; Option < 1) ->
        write("Error: Incorrect Option. Sorry! Start from beginning.");
        guide_course(Option)
    ),
    nl.
