//This file is included but also auto-processed
//It serves as list for defined internal constants that relate to automatically generated elements.
//--STRUCTURE_OBJECTTYPE
//List of the id numbers of all instances, for flaglists, undo buffer lists, and undo command definitions
#define STRUCTURE_OBJECTTYPE_None 0
#define STRUCTURE_OBJECTTYPE_n 1
#define STRUCTURE_OBJECTTYPE_b 2
#define STRUCTURE_OBJECTTYPE_t 3
#define STRUCTURE_OBJECTTYPE_arrow 4
#define STRUCTURE_OBJECTTYPE_graphic 5
#define STRUCTURE_OBJECTTYPE_curve 6
#define STRUCTURE_OBJECTTYPE_matrix 7
#define STRUCTURE_OBJECTTYPE_spectrum 8
#define STRUCTURE_OBJECTTYPE_tlcplate 9
#define STRUCTURE_OBJECTTYPE_hatch 10
//--TELESCOPE_ELEMENTTYPE
//Numbers may not have interruption, but dummies are allowed.
#define TELESCOPE_ELEMENTTYPE_None 0
//better do not use ... 
#define TELESCOPE_ELEMENTTYPE_Empty 1
//empty, tells the sackdown to discard this element.
#define TELESCOPE_ELEMENTTYPE_Annotation 2
#define TELESCOPE_ELEMENTTYPE_s 3
//Text. You can have multiple ones. Each one contains one style information, and multiple bookstaves
#define TELESCOPE_ELEMENTTYPE_PolygonPoint 4
//Points to atom ID's.
#define TELESCOPE_ELEMENTTYPE_Bezierpoint 5
//You can have only one in most objects, or more if it is a multi-trail object
//Contains a bezierpoint list. However, there are only as many elements valid as specified in <count>
#define TELESCOPE_ELEMENTTYPE_Vectorpoint 6
//not defined yet. define with care to ensure uttermost compatibility
#define TELESCOPE_ELEMENTTYPE_ID_Reference 7
//contains an id with purposes.
#define TELESCOPE_ELEMENTTYPE_ContentList 8
//contains a id list
#define TELESCOPE_ELEMENTTYPE_s_f 9
//contains a format list for atom symbols
#define TELESCOPE_ELEMENTTYPE_Symbol 10
//a charge symbol placed next to the atom
#define TELESCOPE_ELEMENTTYPE_tlclane 11
//a TLC or electrophoresis lane
#define TELESCOPE_ELEMENTTYPE_wildcard 12
//attribute that this position can match multiple different element types
//--TELESCOPE_IDPURPOSE
#define TELESCOPE_IDPURPOSE_None 0
//better do not use ...
#define TELESCOPE_IDPURPOSE_Owner 1
//A group it belongs to. Any object should only have at most one, or the user should prepare for frazzlement.
#define TELESCOPE_IDPURPOSE_LinePartner 2
//the >>other bracket<< etc.
#define TELESCOPE_IDPURPOSE_FurtherGroup 3
//For selection groups or something.
#define TELESCOPE_IDPURPOSE_TextField 4
//Textfield id for this atom
//--TELESCOPE_WILDCARD
#define TELESCOPE_WILDCARD_UNDEFINED 0
//better do not use ...
#define TELESCOPE_WILDCARD_SYMBOL 1
//A Symbol for chemical wildcard belongs here. see enums.hxx, ExternalConnectionType enum for values in the following 32-bit integer
#define TELESCOPE_WILDCARD_NUMBERED 2
//A Symbol with a number on it, to tell them from each other. See above for structure
#define TELESCOPE_WILDCARD_BOOKSTAVE 3
//A string like "R" or "X" or "Ar" that tells what type of atoms the wildcard means. NUL-terminated String follows.
#define TELESCOPE_WILDCARD_SLASHLIST 4
//A slash-separated list of possible matchings. NUL-terminated String follows.
