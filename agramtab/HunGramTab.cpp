#include "HunGramTab.h"

const static std::string HungarianPartOfSpeech[] =
{
	"NOUN",
	"VERB",
	"ADJ",
	"ADV",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM",
	"PART",
	"DET"
};

const static std::string HungarianGrammems[] =
{
	"sg", "pl",
	"nom", "acc", "dat", "gen", "ins", "cau", "tra", "ess",
	"ill", "ine", "ela", "sub", "del", "all", "ade", "abl", "ter", "lat",
	"p1", "p2", "p3",
	"pres", "past", "fut",
	"cond", "impv", "ind", "sbjv",
	"def", "indef",
	"inf", "ptcp",
	"pos", "comp", "sup"
};


CHunGramTab::CHunGramTab()
{
	assert(sizeof(HungarianGrammems) / sizeof(std::string) == HUNGARIAN_GRAMMEMS_COUNT);
	assert(sizeof(HungarianPartOfSpeech) / sizeof(std::string) == HUNGARIAN_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphHungarian;
}

CHunGramTab::~CHunGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CHunGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CHunGramTab::GetPartOfSpeechesCount() const {
	return HUNGARIAN_PART_OF_SPEECH_COUNT;
}

const char* CHunGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return HungarianPartOfSpeech[i].c_str();
}

const char* CHunGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CHunGramTab::GetGrammemsCount() const {
	return HUNGARIAN_GRAMMEMS_COUNT;
}

const char* CHunGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return HungarianGrammems[i].c_str();
}

size_t CHunGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CHunGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	// Hungarian has no gender, only number agreement
	grammems_mask_t number1 = g1 & ((1ULL << hunSg) | (1ULL << hunPl));
	grammems_mask_t number2 = g2 & ((1ULL << hunSg) | (1ULL << hunPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CHunGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << hunP1) | (1ULL << hunP2) | (1ULL << hunP3));
	grammems_mask_t person2 = g2 & ((1ULL << hunP1) | (1ULL << hunP2) | (1ULL << hunP3));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << hunSg) | (1ULL << hunPl));
	grammems_mask_t number2 = g2 & ((1ULL << hunSg) | (1ULL << hunPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CHunGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CHunGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunNOUN)) != 0;
}

bool CHunGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunADJ)) != 0;
}

bool CHunGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CHunGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunPRON)) != 0;
}

bool CHunGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return false;
}

bool CHunGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses);
}

bool CHunGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunNUM)) != 0;
}

bool CHunGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunVERB)) != 0;
}

bool CHunGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CHunGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CHunGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << hunADV)) != 0;
}

bool CHunGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << hunPRON)) != 0;
}

bool CHunGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CHunGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CHunGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CHunGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CHunGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CHunGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CHunGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == hunNOUN || p == hunADJ || p == hunVERB || p == hunADV;
}
