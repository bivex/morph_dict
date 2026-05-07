#pragma once

#include "agramtab.h"

enum UkrPartOfSpeechEnum
{
    uNOUN = 0,
    uADJ_FULL = 1,
    uVERB = 2,
    uPRONOUN = 3,
    uPRONOUN_P = 4,
    uPRONOUN_PREDK = 5,
    uNUMERAL = 6,
    uNUMERAL_P = 7,
    uADV = 8,
    uPREDK = 9,
    uPREP = 10,
    uPOSL = 11,
    uCONJ = 12,
    uINTERJ = 13,
    uINP = 14,
    uPHRASE = 15,
    uPARTICLE = 16,
    uADJ_SHORT = 17,
    uPARTICIPLE = 18,
    uADVERB_PARTICIPLE = 19,
    uPARTICIPLE_SHORT = 20,
    uINFINITIVE = 21,
    UKRAINIAN_PART_OF_SPEECH_COUNT = 22
};

enum UkrainianGrammemsEnum {
    uPlural = 0,
    uSingular = 1,
    uAllNumbers = (((uint64_t)1 << uSingular) | ((uint64_t)1 << uPlural)),

    uNominativ = 2,
    uGenitiv = 3,
    uDativ = 4,
    uAccusativ = 5,
    uInstrumentalis = 6,
    uLocativ = 7,
    uVocativ = 8,
    uAllCases = (((uint64_t)1 << uNominativ) | ((uint64_t)1 << uGenitiv) | ((uint64_t)1 << uDativ) | ((uint64_t)1 << uAccusativ) | ((uint64_t)1 << uInstrumentalis) | ((uint64_t)1 << uVocativ) | ((uint64_t)1 << uLocativ)),

    uMasculinum = 9,
    uFeminum = 10,
    uNeutrum = 11,
    uAllGenders = (((uint64_t)1 << uMasculinum) | ((uint64_t)1 << uFeminum) | ((uint64_t)1 << uNeutrum)),

    uPresentTense = 13,
    uFutureTense = 14,
    uPastTense = 15,
    uAllTimes = (((uint64_t)1 << uPresentTense) | ((uint64_t)1 << uFutureTense) | ((uint64_t)1 << uPastTense)),

    uFirstPerson = 16,
    uSecondPerson = 17,
    uThirdPerson = 18,
    uAllPersons = (((uint64_t)1 << uFirstPerson) | ((uint64_t)1 << uSecondPerson) | ((uint64_t)1 << uThirdPerson)),

    uImperative = 19,

    uAnimative = 20,
    uNonAnimative = 21,
    uAllAnimative = (((uint64_t)1 << uAnimative) | ((uint64_t)1 << uNonAnimative)),

    uComparative = 22,

    uPerfective = 23,
    uNonPerfective = 24,

    uNonTransitive = 25,
    uTransitive = 26,

    uActiveVoice = 27,
    uPassiveVoice = 28,

    uIndeclinable = 29, 
    uInitialism = 30,	

    uPatronymic = 31, 

    uToponym = 32,
    uOrganisation = 33,

    uQualitative = 34,
    uDeFactoSingTantum = 35,

    uInterrogative = 36,
    uDemonstrative = 37,

    uName = 38,
    uSurName = 39,
    uImpersonal = 40,
    uSlang = 41,
    uMisprint = 42,
    uColloquial = 43,
    uPossessive = 44,
    uArchaism = 45,
    uSecondCase = 46,
    uPoetry = 47,
    uProfession = 48,
    uSuperlative = 49,
    uPositive = 50,
    UkrainianGrammemsCount = 51
};

namespace UkrClauseType {
    enum class ClauseHeadType {
        FiniteVerb = 0,           // ГЛ_ЛИЧН
        Infinitive = 1,           // ИНФ
        Participial = 2,          // ПРЧ
        AdverbialParticiple = 3,  // ДПР
        Predicative = 4,          // ПРЕДК
        NounPredicate = 5,        // ІМЕННИЙ_ПРЕДИКАТ (если определим)
        NumeralPredicate = 6,     // ЧИСЛІВНИЙ_ПРЕДИКАТ (если определим)
        CategoryState = 7,        // КАТ_СОСТ
        Gerund = 8,               // ДІЄПРИСЛІВНИК (герундій)
        PassivePredicate = 9,     // ПАСИВНИЙ_ПРЕДИКАТ
        AnalyticVerb = 10,        // АНАЛІТИЧНЕ_ДІЄСЛОВО
        Phraseological = 11,      // ФРАЗЕОЛОГІЗМ
        UndetachedAdjParticiple = 12  // НЕВІДОКРЕМЛЕНЕ_ПРИЧАСТЯ
    };

    enum class PredicateType {
        PersonalVerb = 13,
        ImpersonalVerb = 14,
        Copula = 15,
        Ellipsis = 16,
        NominativeSent = 17
    };

    enum class ClauseFunction {
        Introductory = 18,
        Dash = 19,
        Comparative = 20,
        Vocative = 21,
        DirectSpeech = 22,
        Parenthesis = 23,
        Conditional = 24,
        Imperative = 25,
        Question = 26,
        Exclamative = 27,
        Interjection = 28,
        ModalWord = 29,
        Particle = 30
    };

    const int Count = 31;
}

struct CUkrPopularGramCodes {
    std::string m_ProductiveNoun;
    std::string m_ProductiveSingNoun;
    std::string m_InanimIndeclNoun;
    std::string m_MasAbbrNoun;
};

class CUkrGramTab : public CAgramtab {
    const static size_t uStartUp = 0x4141; 
    const static size_t uEndUp = 0x7A7B;  
    const static size_t uMaxGrmCount = uEndUp - uStartUp; 
protected:
    void InitLanguageSpecific(rapidjson::Document& doc) override;
public:
    CAgramtabLine *Lines[uMaxGrmCount];
    CUkrPopularGramCodes m_PopularGramCodes;

    CUkrGramTab();
    ~CUkrGramTab() override;
    void LoadFromRegistry() override;

    const CUkrPopularGramCodes& GramCodes() const { return m_PopularGramCodes; };

    part_of_speech_t GetPartOfSpeechesCount() const override;
    const char *GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na = naDefault) const override;
    const char* GetPartOfSpeechStrLong(part_of_speech_t i) const  override;
    grammem_t GetGrammemsCount() const override;
    const char *GetGrammemStr(size_t i, NamingAlphabet na=naDefault) const override;
    size_t GetMaxGrmCount() const override;
    CAgramtabLine *&GetLine(size_t LineNo) override;
    const CAgramtabLine *GetLine(size_t LineNo) const override;
    size_t GramcodeToLineIndex(const char *s) const override;
    std::string LineIndexToGramcode(uint16_t i) const override;
    bool ProcessPOSAndGrammems(const char *tab_str, part_of_speech_t &PartOfSpeech, grammems_mask_t &grammems) const override;
    bool GleicheCase(const char *gram_code_noun, const char *gram_code_adj) const override;
    bool GleicheCaseNumber(const char *gram_code1, const char *gram_code2) const override;
    grammems_mask_t GleicheGenderNumberCase(const char *common_gram_code_noun, const char *gram_code_noun,
                                            const char *gram_code_adj) const override;
    bool GleicheGenderNumber(const char *gram_code1, const char *gram_code2) const override;
    bool GleicheSubjectPredicate(const char *gram_code1, const char *gram_code2) const override;
    long GetClauseTypeByName(const char *TypeName) const override;
    const char *GetClauseNameByType(long type) const override;
    bool IsStrongClauseRoot(part_of_speech_mask_t poses) const override;
    bool IsMorphNoun(part_of_speech_mask_t poses) const override;
    bool is_morph_adj(part_of_speech_mask_t poses) const override;
    bool is_morph_participle(part_of_speech_mask_t poses) const override;
    bool is_morph_pronoun(part_of_speech_mask_t poses) const override;
    bool is_morph_pronoun_adjective(part_of_speech_mask_t poses) const override;
    bool is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const override;
    bool is_numeral(part_of_speech_mask_t poses) const override;
    bool is_verb_form(part_of_speech_mask_t poses) const override;
    bool is_infinitive(part_of_speech_mask_t poses) const override;
    bool is_morph_predk(part_of_speech_mask_t poses) const override;
    bool is_morph_adv(part_of_speech_mask_t poses) const override;
    bool is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const override;
    bool IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const override;
    bool IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const override;
    bool IsStandardParamAbbr(const char *WordStrUpper) const override;
    bool FilterNounNumeral(std::string &gcNoun, const std::string &gcNum, grammems_mask_t &grammems) const override;
    grammems_mask_t ChangeGleicheAncode1(GrammemCompare CompareFunc, const std::string &wordGramCodes, std::string &groupGramCodes, const grammems_mask_t wordGrammems) const override;
    bool PartOfSpeechIsProductive(part_of_speech_t p) const override;

    std::string FilterGramCodes1(const std::string& gram_codes, grammems_mask_t positive, grammems_mask_t negative) const;
};
