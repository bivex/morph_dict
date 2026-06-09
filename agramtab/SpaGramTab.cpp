#include "SpaGramTab.h"

const static std::string SpanishPartOfSpeech[] =
{
	"NOUN",
	"ADJ",
	"VERB",
	"ADV",
	"DET",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM"
};

const static std::string SpanishGrammems[] =
{
	"sg", "pl", "masc", "fem", "neut",
	"nom", "acc", "dat", "gen", "voc",
	"pres", "past", "fut", "impf", "pfv",
	"cond", "ind", "sbjv", "impv",
	"p1", "p2", "p3",
	"inf", "ger", "ptcp",
	"comp", "sup", "pos",
	"form", "infm", "neg", "pro"
};


CSpaGramTab::CSpaGramTab()
{
	assert(sizeof(SpanishGrammems) / sizeof(std::string) == SPANISH_GRAMMEMS_COUNT);
	assert(sizeof(SpanishPartOfSpeech) / sizeof(std::string) == SPANISH_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphSpanish;
}

CSpaGramTab::~CSpaGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CSpaGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CSpaGramTab::GetPartOfSpeechesCount() const {
	return SPANISH_PART_OF_SPEECH_COUNT;
}

const char* CSpaGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return SpanishPartOfSpeech[i].c_str();
}

const char* CSpaGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CSpaGramTab::GetGrammemsCount() const {
	return SPANISH_GRAMMEMS_COUNT;
}

const char* CSpaGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return SpanishGrammems[i].c_str();
}

size_t CSpaGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CSpaGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t gender1 = g1 & ((1ULL << sgMasculinum) | (1ULL << sgFeminum) | (1ULL << sgNeutrum));
	grammems_mask_t gender2 = g2 & ((1ULL << sgMasculinum) | (1ULL << sgFeminum) | (1ULL << sgNeutrum));
	if (gender1 && gender2 && gender1 != gender2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << sgSingular) | (1ULL << sgPlural));
	grammems_mask_t number2 = g2 & ((1ULL << sgSingular) | (1ULL << sgPlural));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CSpaGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << sgFirstPerson) | (1ULL << sgSecondPerson) | (1ULL << sgThirdPerson));
	grammems_mask_t person2 = g2 & ((1ULL << sgFirstPerson) | (1ULL << sgSecondPerson) | (1ULL << sgThirdPerson));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << sgSingular) | (1ULL << sgPlural));
	grammems_mask_t number2 = g2 & ((1ULL << sgSingular) | (1ULL << sgPlural));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CSpaGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CSpaGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaNOUN)) != 0;
}

bool CSpaGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaADJ)) != 0;
}

bool CSpaGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CSpaGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaPRON)) != 0;
}

bool CSpaGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaDET)) != 0;
}

bool CSpaGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses) || is_morph_pronoun_adjective(poses);
}

bool CSpaGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaNUM)) != 0;
}

bool CSpaGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaVERB)) != 0;
}

bool CSpaGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CSpaGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CSpaGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << spaADV)) != 0;
}

bool CSpaGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << spaPRON)) != 0;
}

bool CSpaGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CSpaGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CSpaGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CSpaGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CSpaGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CSpaGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CSpaGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == spaNOUN || p == spaADJ || p == spaVERB || p == spaADV;
}
