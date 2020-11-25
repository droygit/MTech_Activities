:- dynamic guide_course/1.
:- dynamic suitable_be_course/3.

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


show_all_courses:-
    undergraduate_courses(),
    nl.

subject_name(ISubject):-
    write(ISubject),
    write(": ").


suitable_be_course(IMathMarks1,IPhyMarks2, 2):-
    (
    (IMathMarks1 > 70 , IPhyMarks2 > 60) ->
        show_course("B.E.", "Civil Engineering"),
        show_course("B.E.", "Mechanical Engineering"),
        show_course("B.E.", "Computer Science")
    ).

suitable_be_course(IMathMarks1,IPhyMarks2, 1):-
    (
    (IMathMarks1 > 70 , IPhyMarks2 > 70) ->
        show_course("B.E.", "Civil Engineering"),
        show_course("B.E.", "Mechanical Engineering"),
        show_course("B.E.", "Computer Science"),
        show_course("B.E.", "Electrical and Electronics Engineering")
    ).

suitable_be_course(IMathMarks1,IPhyMarks2,IChemMarks, 3):-
    (
    (IMathMarks1 > 70 , IPhyMarks2 > 60 , IChemMarks > 70) ->
        show_course("B.E.", "Chemical Engineering")
    ).

%TODO: Use this function.
cutoff_marks(IName,ISubject,IMarks,ICuttOff):-
    (
    (IMarks < ICuttOff) ->
        write("Sorry "),
        write(IName),
        write("! You could't clear the cut off marks of "),
        write(ISubject);
        write("Congratulation "),
        write(IName),
        write("! You clear the cut off marks of "),
        write(ISubject)
    ),
    nl.

guide_course(IName):-
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
        write("Sorry "),
        write(IName),
        write("! You are not eligible for any of the courses because Total Marks is less than 60%.");
        (
        (EngMarks < 50) ->
            write("Sorry "),
            write(IName),
            write("! You are not eligible for any of the courses because English is less than 50%.");
            write(IName),
            write(", You are eligible for follwoing subjects - "),nl,
            (
            (suitable_be_course(MathMarks,PhyMarks,1) ;
            suitable_be_course(MathMarks,PhyMarks,2) ; 
            suitable_be_course(MathMarks,PhyMarks, ChemMarks, 3)) ->
                nl;
                write("Sorry! You are not eligible for any available courses.")
            )
        )
    ),
    %write("Done!"),
    nl.

show_degree_info(IName):-
    guide_course(IName),
    nl.
