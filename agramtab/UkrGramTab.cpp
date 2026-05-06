#include "UkrGramTab.h"

struct TMorphConstant {
    std::string Cyrillic;
    std::string Latin;
    std::string CyrillicLong;
};

const static TMorphConstant UkrainianPartOfSpeech[] = {
    {"С", "N", "СУЩЕСТВИТЕЛЬНОЕ"}, 
    {"П", "A", "ПРИЛАГАТЕЛЬНОЕ"}, 
    {"Г", "V", "ЛИЧНАЯ ФОРМА"}, 
    {"МС","PRON", "МЕСТОИМЕНИЕ"}, 
    {"МС-П","PA", "МЕСТОИМЕНИЕ-ПРИЛАГАТЕЛЬНОЕ"}, 
    {"МС-ПРЕДК","P_PRED", "МЕСТОИМЕНИЕ-ПРЕДИКАТИВ"}, 
    {"ЧИСЛ", "NUM", "ЧИСЛИТЕЛЬНОЕ"}, 
    {"ЧИСЛ-П","ORD_NUM", "ПОРЯДКОВОЕ ЧИСЛИТЕЛЬНОЕ"}, 
    {"Н","ADV", "НАРЕЧИЕ"}, 
    {"ПРЕДК", "PRED", "ПРЕДИКАТИВ"}, 
    {"ПРЕДЛ", "PREP", "ПРЕДЛОГ"}, 
    {"ПОСЛ", "POSL", "ПОСЛЕЛОГ"}, 
    {"СОЮЗ", "CONJ", "СОЮЗ"}, 
    {"МЕЖД", "INT", "МЕЖДОМЕТИЕ"}, 
    {"ВВОДН", "INP", "ВВОДНОЕ СЛОВО"},
    {"ФРАЗ", "COLLOC", "ФРАЗЕОЛОГИЗМ"}, 
    {"ЧАСТ", "PARTICLE", "ЧАСТИЦА"}, 
    {"КР_ПРИЛ", "ADJ_SHORT", "КР_ПРИЛ"},  
    {"ПРИЧАСТИЕ", "PARTICIPLE", "ПРИЧАСТИЕ"}, 
    {"ДЕЕПРИЧАСТИЕ","ADV_PARTICIPLE", "ДЕЕПРИЧАСТИЕ"}, 
    {"КР_ПРИЧАСТИЕ","PARTICIPLE_SHORT", "КРАТКОЕ ПРИЧАСТИЕ"}, 
    {"ИНФИНИТИВ", "INFINITIVE", "ИНФИНИТИВ"}  
};

const static TMorphConstant Grammems[] = {
    {"мн", "pl"},
    {"ед", "sg"},
    {"им", "nom"},
    {"рд", "gen"},
    {"дт", "dat"},
    {"вн", "acc"},
    {"тв", "ins"},
    {"пр", "loc"}, 
    {"зв", "voc"},
    {"мр", "mas"},
    {"жр", "fem"},
    {"ср", "neu"},
    {"наст", "pres"},
    {"буд", "fut"},
    {"прош", "past"},
    {"1л", "1p"},
    {"2л", "2p"},
    {"3л", "3p"},
    {"пов", "impe"},
    {"од", "anim"},
    {"но", "inanim"},
    {"сравн", "comp"},
    {"св", "perf"},
    {"нс", "imp"},
    {"нп", "ntr"},
    {"пе", "tr"},
    {"дст", "act"},
    {"стр", "pass"},
    {"0", "ind"},
    {"аббр", "abbr"},
    {"отч", "patr"},
    {"лок", "topo"},
    {"орг", "org"},
    {"кач", "qual"},
    {"дфст", "st"},
    {"вопр", "ques"},
    {"указат", "dem"},
    {"имя", "name"},
    {"фам", "surn"},
    {"безл", "imper"},
    {"жарг", "slang"},
    {"опч", "err"},
    {"разг", "coll"},
    {"притяж", "poss"},
    {"арх", "arch"},
    {"2", "case2"},
    {"поет", "poet"},
    {"проф", "prof"},
    {"прев", "supr"},
    {"полн", "pos"}
};

CUkrGramTab::CUkrGramTab() : CAgramtab() {
    m_Language = morphUkrainian;
    memset(Lines, 0, sizeof(Lines));
}

CUkrGramTab::~CUkrGramTab() {
    for (size_t i = 0; i < uMaxGrmCount; i++)
        if (Lines[i]) delete Lines[i];
}

void CUkrGramTab::InitLanguageSpecific(rapidjson::Document& doc) {
}

void CUkrGramTab::LoadFromRegistry() {
    ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CUkrGramTab::GetPartOfSpeechesCount() const {
    return UKRAINIAN_PART_OF_SPEECH_COUNT;
}

const char* CUkrGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
    return (na == naNational) ? UkrainianPartOfSpeech[i].Cyrillic.c_str() : UkrainianPartOfSpeech[i].Latin.c_str();
}

const char* CUkrGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
    return UkrainianPartOfSpeech[i].CyrillicLong.c_str();
}

grammem_t CUkrGramTab::GetGrammemsCount() const {
    return UkrainianGrammemsCount;
}

const char* CUkrGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
    return (na == naNational) ? Grammems[i].Cyrillic.c_str() : Grammems[i].Latin.c_str();
}

size_t CUkrGramTab::GetMaxGrmCount() const {
    return uMaxGrmCount;
}

CAgramtabLine*& CUkrGramTab::GetLine(size_t LineNo) {
    return Lines[LineNo];
}

const CAgramtabLine* CUkrGramTab::GetLine(size_t LineNo) const {
    return Lines[LineNo];
}

size_t CUkrGramTab::GramcodeToLineIndex(const char* s) const {
    return (unsigned char)s[0] * 0x100 + (unsigned char)s[1] - uStartUp;
}

std::string CUkrGramTab::LineIndexToGramcode(uint16_t i) const {
    uint16_t code = i + uStartUp;
    char s[3];
    s[0] = code / 0x100;
    s[1] = code % 0x100;
    s[2] = 0;
    return s;
}

bool CUkrGramTab::ProcessPOSAndGrammems(const char* tab_str, part_of_speech_t& PartOfSpeech, grammems_mask_t& grammems) const {
    return CAgramtab::ProcessPOSAndGrammems(tab_str, PartOfSpeech, grammems);
}

// --- Ukrainian Gleiche helper functions ---
// Ukrainian has the same case/gender/number system as Russian.
// Cases: nom, gen, dat, acc, ins, loc, voc
// Genders: masc, fem, neut
// Numbers: sg, pl

static bool UkrGleicheGenderNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return ((_QM(uAllNumbers) & l1->m_Grammems & l2->m_Grammems) > 0)
        && (((l1->m_Grammems & l2->m_Grammems & _QM(uPlural)) > 0)
            || ((_QM(uAllGenders) & l1->m_Grammems & l2->m_Grammems) > 0));
}

static bool UkrGleicheCase(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return (_QM(uAllCases) & l1->m_Grammems & l2->m_Grammems) > 0;
}

static bool UkrGleicheCaseNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return ((_QM(uAllCases) & l1->m_Grammems & l2->m_Grammems) > 0)
        && ((_QM(uAllNumbers) & l1->m_Grammems & l2->m_Grammems) > 0);
}

static bool UkrGleichePersonNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return (_QM(uAllPersons) & l1->m_Grammems & l2->m_Grammems) > 0
        && (_QM(uAllNumbers) & l1->m_Grammems & l2->m_Grammems) > 0;
}

static bool UkrGleicheSubjectPredicate(const CAgramtabLine* subj_l, const CAgramtabLine* verb_l) {
    const grammems_mask_t& subj = subj_l->m_Grammems;
    const grammems_mask_t& verb = verb_l->m_Grammems;

    if (!(subj & _QM(uNominativ)))
        return false;

    if ((verb & _QM(uPastTense)) > 0
        || verb_l->m_PartOfSpeech == uADJ_SHORT
        || verb_l->m_PartOfSpeech == uPARTICIPLE_SHORT)
    {
        if (subj & (_QM(uFirstPerson) | _QM(uSecondPerson)))
            return (verb & subj & _QM(uPlural)) > 0
                || ((verb & (_QM(uMasculinum) | _QM(uFeminum))) > 0
                    && (verb & subj & _QM(uSingular)) > 0);
        else
            return UkrGleicheGenderNumber(subj_l, verb_l);
    }
    else if ((verb & _QM(uPresentTense)) > 0 || (verb & _QM(uFutureTense)) > 0) {
        if ((subj & (_QM(uFirstPerson) | _QM(uSecondPerson))) > 0
            || (verb & (_QM(uFirstPerson) | _QM(uSecondPerson))) > 0)
            return UkrGleichePersonNumber(subj_l, verb_l);
        else
            return (_QM(uAllNumbers) & subj & verb) > 0;
    }
    else if (verb & _QM(uImperative)) {
        return (subj & _QM(uSecondPerson)) > 0
            && (_QM(uAllNumbers) & subj & verb) > 0;
    }

    return false;
}

static bool UkrGleicheGenderNumberCase(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((_QM(uAllCases) & g1 & g2) > 0)
        && ((_QM(uAllNumbers) & g1 & g2) > 0)
        && (((_QM(uAllGenders) & g1 & g2) > 0)
            || ((_QM(uAllGenders) & g1) == 0)
            || ((_QM(uAllGenders) & g2) == 0));
}

static bool UkrGleicheGenderNumberCaseAnim(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((_QM(uAllCases) & g1 & g2) > 0)
        && ((_QM(uAllNumbers) & g1 & g2) > 0)
        && (((_QM(uAnimative) & g2) > 0) || ((_QM(uAllAnimative) & g2) == 0))
        && (((_QM(uAllGenders) & g1 & g2) > 0)
            || ((_QM(uAllGenders) & g1) == 0)
            || ((_QM(uAllGenders) & g2) == 0));
}

static bool UkrGleicheGenderNumberCaseInanim(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((_QM(uAllCases) & g1 & g2) > 0)
        && ((_QM(uAllNumbers) & g1 & g2) > 0)
        && (((_QM(uNonAnimative) & g2) > 0) || ((_QM(uAllAnimative) & g2) == 0))
        && (((_QM(uAllGenders) & g1 & g2) > 0)
            || ((_QM(uAllGenders) & g1) == 0)
            || ((_QM(uAllGenders) & g2) == 0));
}

bool CUkrGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const {
    return Gleiche(UkrGleicheCase, gram_code_noun, gram_code_adj) != 0;
}

bool CUkrGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheCaseNumber, gram_code1, gram_code2) != 0;
}

grammems_mask_t CUkrGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const {
    if (!common_gram_code_noun || !strcmp(common_gram_code_noun, "??") || strlen(common_gram_code_noun) == 0)
        return Gleiche(UkrGleicheGenderNumberCase, gram_code_noun, gram_code_adj);
    else if ((GetLine(GramcodeToLineIndex(common_gram_code_noun))->m_Grammems & _QM(uNonAnimative)) > 0)
        return Gleiche(UkrGleicheGenderNumberCaseInanim, gram_code_noun, gram_code_adj);
    else if ((GetLine(GramcodeToLineIndex(common_gram_code_noun))->m_Grammems & _QM(uAnimative)) > 0)
        return Gleiche(UkrGleicheGenderNumberCaseAnim, gram_code_noun, gram_code_adj);
    else
        return Gleiche(UkrGleicheGenderNumberCase, gram_code_noun, gram_code_adj);
}

bool CUkrGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheGenderNumber, gram_code1, gram_code2) != 0;
}

bool CUkrGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheSubjectPredicate, gram_code1, gram_code2) != 0;
}

long CUkrGramTab::GetClauseTypeByName(const char* TypeName) const {
    return 0;
}

const char* CUkrGramTab::GetClauseNameByType(long type) const {
    return "";
}

bool CUkrGramTab::IsStrongClauseRoot(part_of_speech_mask_t poses) const {
    return (poses & (1 << uVERB)) != 0;
}

bool CUkrGramTab::IsMorphNoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNOUN)) != 0;
}

bool CUkrGramTab::is_morph_adj(part_of_speech_mask_t poses) const {
    return (poses & (1 << uADJ_FULL)) != 0 || (poses & (1 << uADJ_SHORT)) != 0;
}

bool CUkrGramTab::is_morph_participle(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICIPLE)) != 0;
}

bool CUkrGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN)) != 0;
}

bool CUkrGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN_P)) != 0;
}

bool CUkrGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const {
    return is_morph_adj(poses);
}

bool CUkrGramTab::is_numeral(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNUMERAL)) != 0 || (poses & (1 << uNUMERAL_P)) != 0;
}

bool CUkrGramTab::is_verb_form(part_of_speech_mask_t poses) const {
    return (poses & (1 << uVERB)) != 0 || (poses & (1 << uINFINITIVE)) != 0;
}

bool CUkrGramTab::is_infinitive(part_of_speech_mask_t poses) const {
    return (poses & (1 << uINFINITIVE)) != 0;
}

bool CUkrGramTab::is_morph_predk(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPREDK)) != 0;
}

bool CUkrGramTab::is_morph_adv(part_of_speech_mask_t poses) const {
    return (poses & (1 << uADV)) != 0;
}

bool CUkrGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const {
    return (poses & (1 << uPRONOUN)) != 0;
}

bool CUkrGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICLE)) != 0;
}

bool CUkrGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const {
    return IsMorphNoun(poses);
}

bool CUkrGramTab::IsStandardParamAbbr(const char* WordStrUpper) const {
    return false;
}

bool CUkrGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
    return p == uNOUN || p == uADJ_FULL || p == uVERB;
}
